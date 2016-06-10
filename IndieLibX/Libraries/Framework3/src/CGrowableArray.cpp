/*
  Copyright (c) 2015 Tommi Roenty   http://www.tommironty.fi/
  Licensed under The GNU Lesser General Public License, version 2.1:
      http://opensource.org/licenses/LGPL-2.1
*/
#if 0
#include <STX/STXCPP.h>

#include <Framework3/CGrowableArray.h>
//--------------------------------------------------------------------------------------
// Implementation of CGrowableArray
//--------------------------------------------------------------------------------------

// This version doesn't call ctor or dtor.
template<typename TYPE> HRESULT CGrowableArray <TYPE>::SetSizeInternal( int nNewMaxSize )
{
    if( nNewMaxSize < 0 || ( nNewMaxSize > INT_MAX / sizeof( TYPE ) ) )
    {
        assert( false );
        return E_INVALIDARG;
    }

    if( nNewMaxSize == 0 )
    {
        // Shrink to 0 size & cleanup
        if( m_pData )
        {
            free( m_pData );
            m_pData = 0;
        }

        m_nMaxSize = 0;
        m_nSize = 0;
    }
    else if( m_pData == 0 || nNewMaxSize > m_nMaxSize )
    {
        // Grow array
        int nGrowBy = ( m_nMaxSize == 0 ) ? 16 : m_nMaxSize;

        // Limit nGrowBy to keep m_nMaxSize less than INT_MAX
        if( ( UINT )m_nMaxSize + ( UINT )nGrowBy > ( UINT )INT_MAX )
            nGrowBy = INT_MAX - m_nMaxSize;

        nNewMaxSize = __max( nNewMaxSize, m_nMaxSize + nGrowBy );

        // Verify that (nNewMaxSize * sizeof(TYPE)) is not greater than UINT_MAX or the realloc will overrun
        if( sizeof( TYPE ) > UINT_MAX / ( UINT )nNewMaxSize )
            return E_INVALIDARG;

        TYPE* pDataNew = ( TYPE* )realloc( m_pData, nNewMaxSize * sizeof( TYPE ) );
        if( pDataNew == 0 )
            return E_OUTOFMEMORY;

        m_pData = pDataNew;
        m_nMaxSize = nNewMaxSize;
    }

    return S_OK;
}


//--------------------------------------------------------------------------------------
template<typename TYPE> HRESULT CGrowableArray <TYPE>::SetSize( int nNewMaxSize )
{
    int nOldSize = m_nSize;

    if( nOldSize > nNewMaxSize )
    {
        assert( m_pData );
        if( m_pData )
        {
            // Removing elements. Call dtor.

            for( int i = nNewMaxSize; i < nOldSize; ++i )
                m_pData[i].~TYPE();
        }
    }

    // Adjust buffer.  Note that there's no need to check for error
    // since if it happens, nOldSize == nNewMaxSize will be true.)
    HRESULT hr = SetSizeInternal( nNewMaxSize );

    if( nOldSize < nNewMaxSize )
    {
        assert( m_pData );
        if( m_pData )
        {
            // Adding elements. Call ctor.

            for( int i = nOldSize; i < nNewMaxSize; ++i )
                ::new ( &m_pData[i] ) TYPE;
        }
    }

    return hr;
}


//--------------------------------------------------------------------------------------
template<typename TYPE> HRESULT CGrowableArray <TYPE>::Add( const TYPE& value )
{
    HRESULT hr;
    if( FAILED( hr = SetSizeInternal( m_nSize + 1 ) ) )
        return hr;

    assert( m_pData != 0 );

    // Construct the new element
    ::new ( &m_pData[m_nSize] ) TYPE;

    // Assign
    m_pData[m_nSize] = value;
    ++m_nSize;

    return S_OK;
}


//--------------------------------------------------------------------------------------
template<typename TYPE> HRESULT CGrowableArray <TYPE>::Insert( int nIndex, const TYPE& value )
{
    HRESULT hr;

    // Validate index
    if( nIndex < 0 ||
        nIndex > m_nSize )
    {
        assert( false );
        return E_INVALIDARG;
    }

    // Prepare the buffer
    if( FAILED( hr = SetSizeInternal( m_nSize + 1 ) ) )
        return hr;

    // Shift the array
    MoveMemory( &m_pData[nIndex + 1], &m_pData[nIndex], sizeof( TYPE ) * ( m_nSize - nIndex ) );

    // Construct the new element
    ::new ( &m_pData[nIndex] ) TYPE;

    // Set the value and increase the size
    m_pData[nIndex] = value;
    ++m_nSize;

    return S_OK;
}


//--------------------------------------------------------------------------------------
template<typename TYPE> HRESULT CGrowableArray <TYPE>::SetAt( int nIndex, const TYPE& value )
{
    // Validate arguments
    if( nIndex < 0 ||
        nIndex >= m_nSize )
    {
        assert( false );
        return E_INVALIDARG;
    }

    m_pData[nIndex] = value;
    return S_OK;
}


//--------------------------------------------------------------------------------------
// Searches for the specified value and returns the index of the first occurrence
// within the section of the data array that extends from iStart and contains the
// specified number of elements. Returns -1 if value is not found within the given
// section.
//--------------------------------------------------------------------------------------
template<typename TYPE> int CGrowableArray <TYPE>::IndexOf( const TYPE& value, int iStart, int nNumElements )
{
    // Validate arguments
    if( iStart < 0 ||
        iStart >= m_nSize ||
        nNumElements < 0 ||
        iStart + nNumElements > m_nSize )
    {
        assert( false );
        return -1;
    }

    // Search
    for( int i = iStart; i < ( iStart + nNumElements ); i++ )
    {
        if( value == m_pData[i] )
            return i;
    }

    // Not found
    return -1;
}


//--------------------------------------------------------------------------------------
// Searches for the specified value and returns the index of the last occurrence
// within the section of the data array that contains the specified number of elements
// and ends at iEnd. Returns -1 if value is not found within the given section.
//--------------------------------------------------------------------------------------
template<typename TYPE> int CGrowableArray <TYPE>::LastIndexOf( const TYPE& value, int iEnd, int nNumElements )
{
    // Validate arguments
    if( iEnd < 0 ||
        iEnd >= m_nSize ||
        nNumElements < 0 ||
        iEnd - nNumElements < 0 )
    {
        assert( false );
        return -1;
    }

    // Search
    for( int i = iEnd; i > ( iEnd - nNumElements ); i-- )
    {
        if( value == m_pData[i] )
            return i;
    }

    // Not found
    return -1;
}



//--------------------------------------------------------------------------------------
template<typename TYPE> HRESULT CGrowableArray <TYPE>::Remove( int nIndex )
{
    if( nIndex < 0 ||
        nIndex >= m_nSize )
    {
        assert( false );
        return E_INVALIDARG;
    }

    // Destruct the element to be removed
    m_pData[nIndex].~TYPE();

    // Compact the array and decrease the size
    MoveMemory( &m_pData[nIndex], &m_pData[nIndex + 1], sizeof( TYPE ) * ( m_nSize - ( nIndex + 1 ) ) );
    --m_nSize;

    return S_OK;
}

#endif
