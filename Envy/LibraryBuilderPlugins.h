//
// LibraryBuilderPlugins.h
//
// This file is part of Envy (getenvy.com) � 2016-2018
// Portions copyright Shareaza 2002-2008 and PeerProject 2008-2010
//
// Envy is free software. You may redistribute and/or modify it
// under the terms of the GNU Affero General Public License
// as published by the Free Software Foundation (fsf.org);
// version 3 or later at your option. (AGPLv3)
//
// Envy is distributed in the hope that it will be useful,
// but AS-IS WITHOUT ANY WARRANTY; without even implied warranty
// of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
// See the GNU Affero General Public License 3.0 for details:
// (http://www.gnu.org/licenses/agpl.html)
//

#pragma once


class CLibraryBuilderPlugins
{
public:
	static bool ExtractPluginMetadata(DWORD nIndex, const CString& strPath);

private:
	static HRESULT SafeProcess(ILibraryBuilderPlugin* pPlugin, BSTR szPath, ISXMLElement* pElement);
};
