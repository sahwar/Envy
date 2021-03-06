//
// PropItem.cpp
//
// This file is part of Envy (getenvy.com) � 2016-2018
// Portions copyright Shareaza 2002-2005 and PeerProject 2008-2010
// Originally Created by:	Rolandas Rudomanskis
//
// Envy is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation (fsf.org);
// either version 3 of the License, or later version (at your option).
//
// Envy is distributed in the hope that it will be useful,
// but AS-IS WITHOUT ANY WARRANTY; without even implied warranty
// of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
// See the GNU General Public License for more details.
// (http://www.gnu.org/licenses/gpl.html)
//

#include "StdAfx.h"
#include "DocumentReader.h"

////////////////////////////////////////////////////////////////////////
// CDocProperty
//
////////////////////////////////////////////////////////////////////////
// Class Constructor/Destructor
//
CDocProperty::CDocProperty()
{
	ODS(L"CDocProperty::CDocProperty()\n");
	m_bstrName       = NULL;
	m_ulPropID       = 0;
	m_vValue.vt      = VT_EMPTY;
	m_fModified		 = FALSE;
	m_fExternal      = FALSE;
	m_fDeadObj       = FALSE;
	m_fNewItem       = FALSE;
	m_fRemovedItem   = FALSE;
	m_pNextItem      = NULL;
}

CDocProperty::~CDocProperty(void)
{
	ODS(L"CDocProperty::~CDocProperty()\n");
	FREE_BSTR( m_bstrName );
	VariantClear( &m_vValue );
}

////////////////////////////////////////////////////////////////////////
// CustomProperty Implementation
//
////////////////////////////////////////////////////////////////////////
// get_Name
//
HRESULT CDocProperty::get_Name(BSTR *pbstrName)
{
	ODS(L"CDocProperty::get_Name\n");
	if ( pbstrName )
		*pbstrName = ( m_bstrName ? SysAllocString(m_bstrName) : NULL );
	return S_OK;
}

////////////////////////////////////////////////////////////////////////
// get_Type
//
HRESULT CDocProperty::get_Type(dsoFilePropertyType *dsoType)
{
	dsoFilePropertyType lType;
	ODS(L"CDocProperty::get_Type\n");
	switch ( m_vValue.vt & VT_TYPEMASK )
	{
	case VT_BSTR: lType = dsoPropertyTypeString; break;
	case VT_I2:
	case VT_I4:   lType = dsoPropertyTypeLong;   break;
	case VT_R4:
	case VT_R8:   lType = dsoPropertyTypeDouble; break;
	case VT_BOOL: lType = dsoPropertyTypeBool;   break;
	case VT_DATE: lType = dsoPropertyTypeDate;   break;
	default:      lType = dsoPropertyTypeUnknown;
	}
	if ( dsoType ) *dsoType = lType;
	return S_OK;
}

////////////////////////////////////////////////////////////////////////
// get_Value
//
HRESULT CDocProperty::get_Value(VARIANT *pvValue)
{
	ODS(L"CDocProperty::get_Value\n");
	CHECK_NULL_RETURN( pvValue, E_POINTER );
	return VariantCopy( pvValue, &m_vValue );
}

////////////////////////////////////////////////////////////////////////
// put_Value
//
HRESULT CDocProperty::put_Value(VARIANT *pvValue)
{
	VARIANT vtTmp; vtTmp.vt = VT_EMPTY;

	ODS(L"CDocProperty::put_Value\n");
	CHECK_NULL_RETURN( pvValue, E_POINTER );
	CHECK_FLAG_RETURN( (m_fDeadObj || m_fRemovedItem), E_INVALIDOBJECT );

	// We don't support arrays (in this sample at least)...
	if ( (pvValue->vt) & VT_ARRAY )
		return E_INVALIDARG;

	// Sanity check of VARTYPE (if it is not one we can save, don't bother)...
	switch ( ((pvValue->vt) & VT_TYPEMASK) )
	{
	case VT_I2:
	case VT_I4:
	case VT_R4:
	case VT_R8:
	case VT_DATE:
	case VT_BSTR:
	case VT_BOOL:
		break;
	default:
		return E_INVALIDARG;
	}

	// Swap out the variant value and set the dirty flag. We make independent
	// copy of the VARIANT (performs indirection as needed)...
	m_fModified = TRUE;
	VariantClear( &m_vValue );
	return VariantCopyInd( &m_vValue, pvValue );
}

////////////////////////////////////////////////////////////////////////
// Remove
//
HRESULT CDocProperty::Remove()
{
	ODS(L"CDocProperty::Remove\n");
	CHECK_FLAG_RETURN( (m_fDeadObj || m_fRemovedItem), E_INVALIDOBJECT );
	VariantClear( &m_vValue );
	m_fRemovedItem = TRUE;
	return S_OK;
}

////////////////////////////////////////////////////////////////////////
// InitProperty
//
HRESULT CDocProperty::InitProperty(BSTR bstrName, PROPID propid, VARIANT* pvData, BOOL fNewItem, CDocProperty* pPreviousItem)
{
	TRACE1("CDocProperty::InitProperty (typeid=%d)\n", pvData->vt);
	ASSERT( m_bstrName == NULL );
	ASSERT( m_ulPropID == 0 );

	m_bstrName = ( (bstrName) ? SysAllocString(bstrName) : NULL );
	m_ulPropID = propid;

	HRESULT hr;
	if ( FAILED( hr = VariantCopy( &m_vValue, pvData ) ) )
		return E_FAIL;

	m_fNewItem = fNewItem;
	m_pNextItem = pPreviousItem;
	return S_OK;
}

////////////////////////////////////////////////////////////////////////
// AppendLink
//
CDocProperty* CDocProperty::AppendLink(CDocProperty* pLinkItem)
{
	CDocProperty* prev = m_pNextItem;
	m_pNextItem = pLinkItem;
	return prev;
}

////////////////////////////////////////////////////////////////////////
// CreateObject - Creates Property with pre-filled information
//
CDocProperty* CDocProperty::CreateObject(BSTR bstrName, PROPID propid, VARIANT* pvData, BOOL fNewItem, CDocProperty* pPreviousItem)
{
	CDocProperty* pitem = new CDocProperty();
	if ( pitem )
	{
		if ( FAILED( pitem->InitProperty( bstrName, propid, pvData, fNewItem, pPreviousItem ) ) )
			pitem = NULL;
	}
	return pitem;
}
