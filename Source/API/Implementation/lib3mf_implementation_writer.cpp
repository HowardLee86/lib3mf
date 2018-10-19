/*++

Copyright (C) 2018 3MF Consortium (Original Author)

All rights reserved.

Redistribution and use in source and binary forms, with or without modification,
are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this
list of conditions and the following disclaimer.
2. Redistributions in binary form must reproduce the above copyright notice,
this list of conditions and the following disclaimer in the documentation
and/or other materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

Abstract: This is a stub class definition of CInternalLib3MFWriter
Interface version: 1.2.2

*/

#include "lib3mf_implementation_writer.hpp"
#include "lib3mf_interfaceexception.hpp"

// Include custom headers here.
#include "Common/Platform/NMR_Platform.h"

using namespace Lib3MF;

/*************************************************************************************************************************
 Class definition of CInternalLib3MFWriter 
**************************************************************************************************************************/

CInternalLib3MFWriter::CInternalLib3MFWriter(NMR::PModelWriter pWriter)
{
	if (!pWriter)
		throw ELib3MFInterfaceException(LIB3MF_ERROR_NOTIMPLEMENTED);
	m_pWriter = pWriter;
}

void CInternalLib3MFWriter::WriteToFile (const std::string sFilename)
{
	if (sFilename.empty())
		throw ELib3MFInterfaceException(NMR_ERROR_INVALIDPOINTER);
	
	setlocale(LC_ALL, "C");

	NMR::PExportStream pStream = NMR::fnCreateExportStreamInstance(sFilename.c_str());
	m_pWriter->exportToStream(pStream);
}

void CInternalLib3MFWriter::GetStreamSize (unsigned long long & nStreamSize)
{
	throw ELib3MFInterfaceException (LIB3MF_ERROR_NOTIMPLEMENTED);
}

void CInternalLib3MFWriter::WriteToBuffer (unsigned int nBufferBufferSize, unsigned int * pBufferNeededCount, unsigned char * pBufferBuffer)
{
	throw ELib3MFInterfaceException (LIB3MF_ERROR_NOTIMPLEMENTED);
}

bool InternalToExternalCallback(Lib3MFProgressCallback pExternalCallback, int nProgressValue, NMR::ProgressIdentifier eProgressIdentifier, void* p)
{
	return pExternalCallback(nProgressValue /100.0, eProgressIdentifier);
}

void CInternalLib3MFWriter::SetProgressCallback (const Lib3MFProgressCallback pProgressCallback)
{
	if (pProgressCallback)
	{
		NMR::Lib3MFProgressCallback internalCallback = std::bind(InternalToExternalCallback,
			pProgressCallback, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);

		m_pWriter->SetProgressCallback(internalCallback, nullptr);
	}
	else
	{
		m_pWriter->SetProgressCallback(nullptr, nullptr);
	}
}

