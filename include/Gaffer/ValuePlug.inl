//////////////////////////////////////////////////////////////////////////
//
//  Copyright (c) 2020, Cinesite VFX Ltd. All rights reserved.
//
//  Redistribution and use in source and binary forms, with or without
//  modification, are permitted provided that the following conditions are
//  met:
//
//      * Redistributions of source code must retain the above
//        copyright notice, this list of conditions and the following
//        disclaimer.
//
//      * Redistributions in binary form must reproduce the above
//        copyright notice, this list of conditions and the following
//        disclaimer in the documentation and/or other materials provided with
//        the distribution.
//
//      * Neither the name of John Haddon nor the names of
//        any other contributors to this software may be used to endorse or
//        promote products derived from this software without specific prior
//        written permission.
//
//  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS
//  IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
//  THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
//  PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
//  CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
//  EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
//  PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
//  PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
//  LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
//  NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
//  SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//
//////////////////////////////////////////////////////////////////////////

#include "fmt/format.h"

namespace Gaffer
{

template<typename T>
boost::intrusive_ptr<const T> ValuePlug::getObjectValue( const IECore::MurmurHash *precomputedHash ) const
{
	IECore::ConstObjectPtr value = getValueInternal( precomputedHash );
	if( value && value->isInstanceOf( T::staticTypeId() ) )
	{
		// Steal the reference from `value`, to avoid incrementing and decrementing the refcount
		// unnecessarily.
		/// \todo Add a move-aware variant of `IECore::runTimeCast()` and use it instead.
		return boost::intrusive_ptr<const T>( static_cast<const T *>( value.detach() ), /* add_ref = */ false );
	}

	throw IECore::Exception( fmt::format(
		"{} : getValueInternal() didn't return expected type (wanted {} but got {}). Is the hash being computed correctly?",
		fullName(), T::staticTypeName(), value ? value->typeName() : "nullptr"
	) );
}

} // namespace Gaffer
