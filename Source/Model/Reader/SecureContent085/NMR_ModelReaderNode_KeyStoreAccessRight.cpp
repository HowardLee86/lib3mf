
/*++

Copyright (C) 2019 3MF Consortium

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

Abstract:

NMR_ModelReaderNode_KeyStoreAccessRight.h defines the Model Reader Node class that is related to <Accessright>.

--*/

#include "Model/Reader/SecureContent085/NMR_ModelReaderNode_KeyStoreAccessRight.h"
#include "Model/Reader/SecureContent085/NMR_ModelReaderNode_KeyStoreCipherValue.h"
#include "Model/Reader/SecureContent085/NMR_ModelReaderNode_KeyStoreKEKParams.h"

#include "Model/Classes/NMR_ModelConstants.h"
#include "Common/NMR_Exception.h"
#include "Common/NMR_Exception_Windows.h"
#include "Common/NMR_StringUtils.h"

namespace NMR {


	CModelReaderNode_KeyStoreAccessRight::CModelReaderNode_KeyStoreAccessRight(CKeyStore * pKeyStore, PModelReaderWarnings pWarnings)
		: CModelReaderNode_KeyStoreBase(pKeyStore, pWarnings)
	{
	}

	PKeyStoreAccessRight CModelReaderNode_KeyStoreAccessRight::getAccessRight()
	{
		return m_accessRight;
	}

	void CModelReaderNode_KeyStoreAccessRight::parseXML(_In_ CXmlReader * pXMLReader)
	{
		// Parse name
		parseName(pXMLReader);

		// Parse attribute
		parseAttributes(pXMLReader);

		// Parse Content
		parseContent(pXMLReader);

		// this can throw for values that are not numbers and it's ok so according to other reader nodes
		try {
			nfUint64 index = fnStringToInt32(m_consumerIndex.c_str());
			if (0 <= index && index < m_pKeyStore->getConsumerCount()) {
				PKeyStoreConsumer c = m_pKeyStore->getConsumerByIndex(index);
				//TODO create accessright with kekparam
				m_accessRight = std::make_shared<CKeyStoreAccessRight>(c, m_encryptionAlgorithm, m_sCipherValue);
			} else {
				m_pWarnings->addException(CNMRException(NMR_ERROR_KEYSTOREINVALIDCONSUMERINDEX), eModelReaderWarningLevel::mrwInvalidMandatoryValue);
			}
		} catch (CNMRException const &) {
			m_pWarnings->addException(CNMRException(NMR_ERROR_KEYSTOREINVALIDCONSUMERINDEX), eModelReaderWarningLevel::mrwInvalidMandatoryValue);
		}
	}

	void CModelReaderNode_KeyStoreAccessRight::OnAttribute(_In_z_ const nfChar * pAttributeName, _In_z_ const nfChar * pAttributeValue)
	{
		__NMRASSERT(pAttributeName);
		__NMRASSERT(pAttributeValue);
		

		if (strcmp(XML_3MF_SECURE_CONTENT_CONSUMER_INDEX, pAttributeName) == 0) {
			if (!m_consumerIndex.empty())
				m_pWarnings->addException(CNMRException(NMR_ERROR_KEYSTOREDUPLICATECONSUMERINDEX), eModelReaderWarningLevel::mrwInvalidMandatoryValue);
			m_consumerIndex = pAttributeValue;
		}
		else
			m_pWarnings->addException(CNMRException(NMR_ERROR_NAMESPACE_INVALID_ATTRIBUTE), mrwInvalidOptionalValue);
	}

	void CModelReaderNode_KeyStoreAccessRight::OnNSChildElement(_In_z_ const nfChar * pChildName, _In_z_ const nfChar * pNameSpace, _In_ CXmlReader * pXMLReader)
	{
		__NMRASSERT(pChildName);
		__NMRASSERT(pXMLReader);
		__NMRASSERT(pNameSpace);

		//TODO Parse CipherData child element

		if (strcmp(pChildName, XML_3MF_ELEMENT_KEKPARAMS) == 0) {
			if (!m_bHasKEKParams) {
				PModelReaderNode_KeyStoreKEKParams pXMLNode = std::make_shared<CModelReaderNode_KeyStoreKEKParams>(m_pKeyStore, m_pWarnings);
				pXMLNode->parseXML(pXMLReader);
				m_sKekParams = pXMLNode->getKekParams();
				m_bHasKEKParams = true;
			}
			else {
				m_pWarnings->addException(CNMRException(NMR_ERROR_NAMESPACE_INVALID_ELEMENT), mrwInvalidOptionalValue);

			}
		}
	}

}
