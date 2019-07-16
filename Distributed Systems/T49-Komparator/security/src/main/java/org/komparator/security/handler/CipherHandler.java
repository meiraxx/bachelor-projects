package org.komparator.security.handler;

import java.io.ByteArrayInputStream;
import java.io.FileNotFoundException;
import java.io.InputStream;
import java.security.InvalidKeyException;
import java.security.KeyStoreException;
import java.security.NoSuchAlgorithmException;
import java.security.PrivateKey;
import java.security.PublicKey;
import java.security.UnrecoverableKeyException;
import java.security.cert.Certificate;
import java.security.cert.CertificateException;
import java.util.Iterator;
import java.util.Set;

import javax.crypto.BadPaddingException;
import javax.crypto.IllegalBlockSizeException;
import javax.crypto.NoSuchPaddingException;
import javax.xml.bind.DatatypeConverter;
import javax.xml.namespace.QName;
import javax.xml.soap.SOAPBody;
import javax.xml.soap.SOAPElement;
import javax.xml.soap.SOAPException;
import javax.xml.ws.handler.MessageContext;
import javax.xml.ws.handler.soap.SOAPHandler;
import javax.xml.ws.handler.soap.SOAPMessageContext;

import org.komparator.security.CertUtil;
import org.komparator.security.CryptoUtil;

import pt.ulisboa.tecnico.sdis.ws.cli.*;

public class CipherHandler implements SOAPHandler<SOAPMessageContext> {

	String PASSWORD = "2s298rzB";

	@Override
	public boolean handleMessage(SOAPMessageContext context) {

		Boolean outbound = (Boolean) context.get(MessageContext.MESSAGE_OUTBOUND_PROPERTY);

		if (outbound) {
			try {
				SOAPBody body = context.getMessage().getSOAPPart().getEnvelope().getBody();
				try {
					CAClient client = new CAClient("http://sec.sd.rnl.tecnico.ulisboa.pt:8081/ca?WSDL");

					String certificate = client.getCertificate("T49_Mediator");
					InputStream input = new ByteArrayInputStream(certificate.getBytes());
					try {
						Certificate cert = CertUtil.getX509CertificateFromStream(input);

						if (body != null) {
							Iterator it = body.getChildElements();

							while (it.hasNext()) {
								SOAPElement element = (SOAPElement) it.next();

								if (element.getElementName().getLocalName().equals("buyCart")) {
									Iterator it1 = element.getChildElements();

									while (it1.hasNext()) {
										SOAPElement child = (SOAPElement) it1.next();

										if (child.getElementName().getLocalName().equals("creditCardNr")) {
											String ccNumber = child.getTextContent();
											byte[] encrCCNumber = DatatypeConverter.parseBase64Binary(ccNumber);
											PublicKey publicKey = CertUtil.getPublicKeyFromCertificate(cert);

											try {
												byte[] byteCriptedCCNumber = CryptoUtil.asymetricCipher(encrCCNumber,
														publicKey);
												String stringCriptedCCNumber = DatatypeConverter
														.printBase64Binary(byteCriptedCCNumber);
												child.removeContents();
												child.addTextNode(stringCriptedCCNumber);

											} catch (InvalidKeyException | NoSuchAlgorithmException
													| NoSuchPaddingException | IllegalBlockSizeException
													| BadPaddingException e) {
												System.out.printf("Failed to apply assimetric cipher because of %s%n",
														e);
												return false;
											}
											return true;
										}
									}
								}
							}
						}
					} catch (CertificateException e1) {
						System.out.printf("Failed to get Certificate because of %s%n", e1);
						return false;
					}
				} catch (CAClientException e2) {
					System.out.printf("Failed to get CAClient because of %s%n", e2);
					return false;
				}

			} catch (SOAPException e) {
				System.out.printf("Failed to get SOAP body because of %s%n", e);
				return false;
			}

		}

		else {

			try {
				SOAPBody body = context.getMessage().getSOAPPart().getEnvelope().getBody();
				
				if (body != null) {
					Iterator it = body.getChildElements();

					while (it.hasNext()) {
						SOAPElement element = (SOAPElement) it.next();

						if (element.getElementName().getLocalName().equals("buyCart")) {
							Iterator it1 = element.getChildElements();

							while (it1.hasNext()) {
								SOAPElement child = (SOAPElement) it1.next();

								if (child.getElementName().getLocalName().equals("creditCardNr")) {
									String ccNumber = child.getTextContent();
									byte[] encrCCNumber = DatatypeConverter.parseBase64Binary(ccNumber);

									try {
										PrivateKey privateKey = CertUtil.getPrivateKeyFromKeyStoreResource("T49_Mediator.jks", PASSWORD.toCharArray(), "t49_mediator", PASSWORD.toCharArray());
										try {
											byte[] byteDecriptedCCNumber = CryptoUtil.asymetricDecipher(encrCCNumber,privateKey);
											String stringDecriptedCCNumber = DatatypeConverter.printBase64Binary(byteDecriptedCCNumber);
											child.removeContents();
											child.addTextNode(stringDecriptedCCNumber);

										} catch (InvalidKeyException | NoSuchAlgorithmException | NoSuchPaddingException | IllegalBlockSizeException | BadPaddingException e) {
											System.out.printf("Failed to apply assimetric cipher because of %s%n", e);
											return false;
										}
									} catch (UnrecoverableKeyException | FileNotFoundException | KeyStoreException e) {
										System.out.printf("Failed to get PrivateKey because of %s%n", e);
										return false;
									}
									return true;
								}
							}
						}
					}
				}
			} catch (SOAPException e) {
				System.out.printf("Failed to get SOAP body because of %s%n", e);
				return false;
			}
		}
		return true;
	}

	public boolean handleFault(SOAPMessageContext context) {
		return true;
	}

	public Set<QName> getHeaders() {
		return null;
	}

	public void close(MessageContext messageContext) {
	}

}
