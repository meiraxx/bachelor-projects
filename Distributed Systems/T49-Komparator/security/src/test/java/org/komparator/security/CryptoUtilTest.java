package org.komparator.security;

import java.io.ByteArrayInputStream;
import java.io.FileNotFoundException;
import java.io.InputStream;
import java.security.KeyStoreException;
import java.security.PrivateKey;
import java.security.PublicKey;
import java.security.UnrecoverableKeyException;
import java.security.cert.Certificate;
import java.security.cert.CertificateException;

import org.junit.Assert;
import org.junit.Before;
import org.junit.Test;

import pt.ulisboa.tecnico.sdis.ws.cli.CAClient;
import pt.ulisboa.tecnico.sdis.ws.cli.CAClientException;

public class CryptoUtilTest {
	
	String PASSWORD = "2s298rzB";
	PrivateKey privateKey;
	PublicKey publicKey;

    @Before
    public void setUp() {
    	CAClient client = null;
		try {
			client = new CAClient("http://sec.sd.rnl.tecnico.ulisboa.pt:8081/ca?WSDL");
			String certificate = client.getCertificate("T49_Mediator");
			InputStream input = new ByteArrayInputStream(certificate.getBytes());
	    	Certificate cert = CertUtil.getX509CertificateFromStream(input);
	        privateKey = CertUtil.getPrivateKeyFromKeyStoreFile("../mediator-ws/src/main/resources/T49_Mediator.jks", PASSWORD.toCharArray(), "t49_mediator", PASSWORD.toCharArray());
	        publicKey = CertUtil.getPublicKeyFromCertificate(cert);
		} catch (CAClientException | UnrecoverableKeyException | FileNotFoundException | KeyStoreException | CertificateException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
    }

    @Test
    public void test() throws Exception {

    	String message = "Random message 101";
    	byte[] encrypt = message.getBytes();
    	byte[] encrypted = CryptoUtil.asymetricCipher(encrypt, publicKey);
      	byte[] decrypted = CryptoUtil.asymetricDecipher(encrypted, privateKey);
      

    	Assert.assertEquals(message,new String(decrypted, "UTF-8"));
    	
    }

}

