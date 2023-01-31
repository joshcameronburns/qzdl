/*
 * This file is part of qZDL
 * Copyright (C) 2007-2012  Cody Harris
 * Copyright (C) 2018-2019  Lcferrum
 * Copyright (C) 2023  spacebub
 * 
 * qZDL is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <QRegularExpression>
#include <utility>
#include "libwad.h"

struct wadinfo_t
{
	int numlumps;
	int infotableofs;
};

struct filelump_t
{
	int filepos;
	int size;
	union
	{
		struct
		{
			qint32 x1;
			qint32 x2;
		};
	};
};

union name8_t
{
	char name[8];
	struct
	{
		qint32 x1;
		qint32 x2;
	};
};

const name8_t things_lump = { 'T', 'H', 'I', 'N', 'G', 'S', '\0', '\0' };
const name8_t textmap_lump = { 'T', 'E', 'X', 'T', 'M', 'A', 'P', '\0' };
const name8_t iwadinfo_lump = { 'I', 'W', 'A', 'D', 'I', 'N', 'F', 'O' };
const name8_t map01_lump = { 'M', 'A', 'P', '0', '1', '\0', '\0', '\0' };

#define NAME8_CMP(f, l) (f.x1==l.x1&&f.x2==l.x2)

DoomWad::DoomWad(QString file) :
	file(std::move(file))
{
}

DoomWad::~DoomWad()
= default;

QStringList DoomWad::getMapNames()
{
	QFile wad(file);
	QStringList map_names;

	if (wad.open(QIODevice::ReadOnly))
	{
		wadinfo_t header{};

		if (wad.read((char*)&header, sizeof(wadinfo_t)) == sizeof(wadinfo_t) && wad.seek(header.infotableofs))
		{
			auto* fileinfo = new filelump_t[header.numlumps];
			qint64 length = (qint64)sizeof(filelump_t) * header.numlumps;

			if (wad.read((char*)fileinfo, length) == length)
			{
				name8_t prev_lump_name{};
				bool prev_lump = false;

				for (int i = 0; i < header.numlumps; i++)
				{
					if (NAME8_CMP(fileinfo[i], things_lump) || NAME8_CMP(fileinfo[i], textmap_lump))
					{
						if (prev_lump)
						{
							map_names << QString(QByteArray::fromRawData(prev_lump_name.name, 8)).toUpper();
							prev_lump = false;
						}
					}
					else
					{
						prev_lump_name.x1 = fileinfo[i].x1;
						prev_lump_name.x2 = fileinfo[i].x2;
						prev_lump = true;
					}
				}
			}

			delete[] fileinfo;
		}

		wad.close();
	}

	return map_names;
}

QString DoomWad::getIwadinfoName()
{
	QFile wad(file);
	QString iwad_name;

	if (wad.open(QIODevice::ReadOnly))
	{
		wadinfo_t header{};

		if (wad.read((char*)&header, sizeof(wadinfo_t)) == sizeof(wadinfo_t) && wad.seek(header.infotableofs))
		{
			auto* fileinfo = new filelump_t[header.numlumps];
			qint64 length = (qint64)sizeof(filelump_t) * header.numlumps;

			if (wad.read((char*)fileinfo, length) == length)
			{
				for (int i = 0; i < header.numlumps; i++)
				{
					if (NAME8_CMP(fileinfo[i], iwadinfo_lump))
					{
						char* iwadinfo = new char[fileinfo[i].size + 1];
						iwadinfo[fileinfo[i].size] = '\0';

						if (wad.seek(fileinfo[i].filepos) && wad.read(iwadinfo, fileinfo[i].size) == fileinfo[i].size)
						{
							QRegularExpression name_re("\\s+Name\\s*=\\s*\"(.+)\"\\s+");
							QRegularExpressionMatch match = name_re.match(iwadinfo, Qt::CaseInsensitive);

							if (match.hasPartialMatch())
								iwad_name = match.captured(1);
						}

						delete[] iwadinfo;
					}
				}
			}

			delete[] fileinfo;
		}

		wad.close();
	}

	return iwad_name;
}

bool DoomWad::isMAPXX()
{
	QFile wad(file);
	bool is_mapxx = false;

	if (wad.open(QIODevice::ReadOnly))
	{
		wadinfo_t header{};

		if (wad.read((char*)&header, sizeof(wadinfo_t)) == sizeof(wadinfo_t) && wad.seek(header.infotableofs))
		{
			auto* fileinfo = new filelump_t[header.numlumps];
			qint64 length = (qint64)sizeof(filelump_t) * header.numlumps;

			if (wad.read((char*)fileinfo, length) == length)
			{
				for (int i = 0; i < header.numlumps; i++)
				{
					//In original ZDoom code only global namespace is checked for 'MAP01' lump
					//Global namespace includes lumps outside of any markers
					if (NAME8_CMP(fileinfo[i], map01_lump))
					{
						is_mapxx = true;
						break;
					}
				}
			}

			delete[] fileinfo;
		}

		wad.close();
	}

	return is_mapxx;
}
