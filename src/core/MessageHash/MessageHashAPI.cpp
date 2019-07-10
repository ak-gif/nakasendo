#include <string.h>
#include <iostream>
#include <vector>
#include <MessageHash/MessageHashAPI.h>
#include <MessageHash/MessageHash.h>
#include <MessageHash/Base64EncDec.h>
#include <MessageHash/Base58EncDec.h>

#include <memory>

#ifdef __EMSCRIPTEN__
#include "utils/emscriptBindings.h"
#endif

//#ifdef __EMSCRIPTEN__
//EMSCRIPTEN_KEEPALIVE
//#endif
//std::vector<uint8_t> returnEmptyVector () {
//  std::vector<uint8_t> v;
//  return v;
//}
#ifdef __EMSCRIPTEN__
EMSCRIPTEN_KEEPALIVE
#endif 
std::vector<uint8_t>  EncodeBase64Ex (const std::vector<uint8_t>& toEnc){
    std::unique_ptr<unsigned char[]> msgPtr ( new unsigned char[toEnc.size()] ) ; 
    std::fill_n(msgPtr.get(), toEnc.size(), 0x00);        
    int index(0);
    for (std::vector<uint8_t>::const_iterator iter = toEnc.begin(); iter != toEnc.end(); ++iter){
            msgPtr.get()[index] = *iter; 
            ++ index ; 
    }
    Base64EncDec encdec ; 
    int sizeEncodedBuffer(0);
    
    std::vector<uint8_t> retVal; 
    std::string testRetStr; 
    std::unique_ptr<unsigned char[]> retValPtr =  encdec.encode (msgPtr, toEnc.size(), 0, sizeEncodedBuffer);
    // We don't want the null terminator now
    for(int i=0;i<(sizeEncodedBuffer-1);++i){
        retVal.push_back(retValPtr.get()[i]);
        testRetStr.push_back (retValPtr.get()[i]);
    }   
    return retVal ;            
}
#ifdef __EMSCRIPTEN__
EMSCRIPTEN_KEEPALIVE 
#endif
std::vector<uint8_t>  DecodeBase64Ex (std::vector<uint8_t> toDec){
    size_t value = 0;
    int strict = 0 ; 
    int * err = new int;  
                
    std::unique_ptr<unsigned char[]> msgPtr ( new unsigned char [toDec.size()+1]);  
    std::fill_n(msgPtr.get(), toDec.size()+1, 0x00);      
    //std::string::const_iterator iter = nonConstMsg.begin();
    int index(0);
    for (std::vector<uint8_t>::const_iterator iter = toDec.begin(); iter != toDec.end(); ++ iter, ++index){
        msgPtr.get()[index] = *iter; 
    }

    Base64EncDec encdec;
    std::unique_ptr<unsigned char[]> decodedValPtr = encdec.decode(msgPtr,value, strict, err);
    std::vector<uint8_t> retVal; 
    std::string strVal; 
    
    if (decodedValPtr != nullptr){
        for (int i=0; i<value;++i){
            retVal.push_back(decodedValPtr.get()[i]);
            strVal.push_back(decodedValPtr.get()[i]);
        }
    }
    return retVal; 
}
#ifdef __EMSCRIPTEN__
EMSCRIPTEN_KEEPALIVE 
#endif
std::vector<uint8_t> EncodeBase58Ex (std::vector<uint8_t> toEnc){
    Base58EncDec encdec ; 
    std::string  encVal =  encdec.encode (toEnc);
    std::vector<uint8_t> retVal; 
    for (std::string::const_iterator iter = encVal.begin(); iter != encVal.end(); ++ iter){
        retVal.push_back(*iter);
    }
    return retVal ; 
}

#ifdef __EMSCRIPTEN__
EMSCRIPTEN_KEEPALIVE 
#endif
std::vector<uint8_t> DecodeBase58Ex (std::vector<uint8_t> toDec){
    std::string msg; 
    for (std::vector<uint8_t>::const_iterator iter = toDec.begin(); iter != toDec.end(); ++ iter){
        msg.push_back(*iter);  
    }        
    Base58EncDec encdec;
    std::vector<uint8_t> decodedVal = encdec.decode(msg);
    return decodedVal ; 
}
#ifdef __EMSCRIPTEN__
EMSCRIPTEN_KEEPALIVE 
#endif
std::vector<uint8_t> EncodeBase58CheckEx (std::vector<uint8_t> toEnc){
    Base58EncDec encdec ; 
    std::string  encVal =  encdec.encodeCheck (toEnc);
    std::vector<uint8_t> retVal;
    for(std::string::const_iterator iter = encVal.begin(); iter != encVal.end(); ++iter){
        retVal.push_back(*iter);
    }
    return retVal; 
}

#ifdef __EMSCRIPTEN__
EMSCRIPTEN_KEEPALIVE 
#endif
std::vector<uint8_t> DecodeBase58CheckEx (std::vector<uint8_t> toDec){
    std::string msg; 
    for (std::vector<uint8_t>::const_iterator iter = toDec.begin(); iter != toDec.end(); ++ iter){
        msg.push_back(*iter);  
    } 
    Base58EncDec encdec;
    std::vector<uint8_t> decodedVal = encdec.decodeCheck(msg);    
    return decodedVal; 
}

#ifdef __EMSCRIPTEN__
EMSCRIPTEN_BINDINGS(moduleHash) {
    emscripten::function("EncodeBase64Ex", &EncodeBase64Ex);  
    emscripten::function("DecodeBase64Ex", &DecodeBase64Ex);
    emscripten::function("EncodeBase58Ex", &EncodeBase58Ex);
    emscripten::function("DecodeBase58Ex", &DecodeBase58Ex);
    emscripten::function("EncodeBase58CheckEx", &EncodeBase58CheckEx);
    emscripten::function("DecodeBase58CheckEx", &DecodeBase58CheckEx);  
  
}
#endif

MESSAGE_HASH_RETURN_TYPE HashMsgSHA256Test (char * msg){
    MessageHash hashMess;
    hashMess.HashSha256 (msg);
#ifdef __EMSCRIPTEN__
        return hashMess.HashHex().c_str(); 
#else
         return hashMess.HashHex ();
#endif             
}

MESSAGE_HASH_RETURN_TYPE HashMsgSHA256 (const std::string& msg)
    {
         MessageHash hashMess;
         hashMess.HashSha256 (msg);
#ifdef __EMSCRIPTEN__
        return hashMess.HashHex().c_str(); 
#else
         return hashMess.HashHex ();
#endif         
    }

MESSAGE_HASH_RETURN_TYPE HashMsg (const std::string& msg, const std::string& hashContext)
    {
         MessageHash hashMess;
         hashMess.Hash(msg,hashContext);
#ifdef __EMSCRIPTEN__
        return hashMess.HashHex().c_str() ; 
#else         
        return hashMess.HashHex ();
#endif        
    }

MessageHash_API std::unique_ptr<unsigned char> HashSha256 ( const std::string& msg ){
    MessageHash hashMess; 
    hashMess.HashSha256(msg);
    //uint8_t retval = hashMess.Value().get(); 
    return std::move(hashMess.Value()); 
}

MESSAGE_HASH_RETURN_TYPE ListHashFunc ()
    {
        MessageHash hashMess;
#ifdef __EMSCRIPTEN__
        return hashMess.ListAvailableHash().c_str() ; 
#else        
        return hashMess.ListAvailableHash () ; 
#endif        
    }


<<<<<<< HEAD
<<<<<<< HEAD
    //MESSAGE_HASH_RETURN_TYPE EncodeBase64 (const std::string& msg)
    MESSAGE_HASH_RETURN_TYPE EncodeBase64 (const unsigned char* msgPtrApi, int msgSize)    
    {
        //std::string msg ( msgPtrApi );
        //std::cout << "msg looks like: " << msg << std::endl ; 
        //std::unique_ptr<unsigned char> msgPtr ( new unsigned char [msg.length()]); 
        std::unique_ptr<unsigned char> msgPtr ( new unsigned char[msgSize] ) ; 
        std::fill_n(msgPtr.get(), msgSize, 0x00);        
        int index(0);

        for ( int i=0; i<msgSize; ++ i)        {
            msgPtr.get()[i] = msgPtrApi[i] ; 
        }
     
=======
    MESSAGE_HASH_RETURN_TYPE EncodeBase64 (const std::unique_ptr<unsigned char[]>& msgPtrApi, int msgSize)    
    {  
>>>>>>> 72e2bba... Commited for Base64 encoding and decoding issues
=======
    MESSAGE_HASH_RETURN_TYPE EncodeBase64 (const std::unique_ptr<unsigned char[]>& msgPtrApi, int msgSize)    
    {  
>>>>>>> 18a824c... Commited for Base64 encoding and decoding issues
        Base64EncDec encdec ; 
        int sizeEncodedBuffer(0);
        std::string retVal;
        std::unique_ptr<unsigned char[]> retValPtr =  encdec.encode (msgPtrApi, msgSize, 0, sizeEncodedBuffer);

        for(int i=0;i<sizeEncodedBuffer;++i){
            retVal.push_back(retValPtr.get()[i]);
        }        
       
#ifdef __EMSCRIPTEN__
        return retVal.c_str() ; 
#else        
        return retVal ; 
#endif        
    }

<<<<<<< HEAD
<<<<<<< HEAD
    //MESSAGE_HASH_RETURN_TYPE DecodeBase64 (const std::string& msg)
    MESSAGE_HASH_RETURN_TYPE DecodeBase64 (const unsigned char* msgPtrApi, int msgSize )    
=======
    std::unique_ptr<unsigned char[]>  DecodeBase64 (const std::string& msgPtrApi, int& msgSize )    
>>>>>>> 72e2bba... Commited for Base64 encoding and decoding issues
=======
    std::unique_ptr<unsigned char[]>  DecodeBase64 (const std::string& msgPtrApi, int& msgSize )    
>>>>>>> 18a824c... Commited for Base64 encoding and decoding issues
    {
        size_t value = 0;
        int strict = 0 ; 
        int * err = new int;  
          
<<<<<<< HEAD
<<<<<<< HEAD
        
        std::unique_ptr<unsigned char> msgPtr ( new unsigned char [msgSize+1]);  
        std::fill_n(msgPtr.get(), msgSize+1, 0x00);      
        for (unsigned int i = 0; i < msgSize;++i){
            msgPtr.get()[i] = msgPtrApi[i] ;
        }        
        Base64EncDec encdec;        
        std::unique_ptr<unsigned char> decodedValPtr = encdec.decode(msgPtr,value, strict, err);
        std::string retVal; 
        if (decodedValPtr != nullptr){
            for (int i=0; i<value;++i){
                retVal.push_back(decodedValPtr.get()[i]);
            }
        }else{retVal="ERROR";}
=======
=======
>>>>>>> 18a824c... Commited for Base64 encoding and decoding issues
        std::unique_ptr<unsigned char[]> msgPtr ( new unsigned char [msgPtrApi.size()+1]);  
        std::fill_n(msgPtr.get(), msgPtrApi.size()+1, 0x00);  
        int i(0);
        for(std::string::const_iterator iter = msgPtrApi.begin(); iter != msgPtrApi.end(); ++iter){ 
            msgPtr.get()[i++] = *iter;
        }        

        Base64EncDec encdec;       
        msgPtr.get()[msgPtrApi.size()+1]='\0';
        std::unique_ptr<unsigned char[]> decodedValPtr = encdec.decode(msgPtr,value, strict, err);
        msgSize = value; 
>>>>>>> 72e2bba... Commited for Base64 encoding and decoding issues
#ifdef __EMSCRIPTEN__
        return retVal.c_str() ;         
#else        
<<<<<<< HEAD
<<<<<<< HEAD
        return retVal ; 
=======
        return decodedValPtr; 
>>>>>>> 72e2bba... Commited for Base64 encoding and decoding issues
=======
        return decodedValPtr; 
>>>>>>> 18a824c... Commited for Base64 encoding and decoding issues
#endif        
    }

    MESSAGE_HASH_RETURN_TYPE EncodeBase58 (const std::string& msg)
    {
        std::vector<uint8_t> vec; 
        for (std::string::const_iterator iter = msg.begin(); iter != msg.end(); ++ iter){
            vec.push_back(*iter);
        }        
        Base58EncDec encdec ; 
        std::string  encVal =  encdec.encode (vec);
#ifdef __EMSCRIPTEN__
        return encVal.c_str() ;         
#else        
        return encVal ; 
#endif        
    }

    MESSAGE_HASH_RETURN_TYPE DecodeBase58 (const std::string& msg)
    {       
        std::string nonConstMsg ( msg ); 
        nonConstMsg = nonConstMsg.erase(nonConstMsg.find_last_not_of("\t\n\v\f\r ")+1);
        
        
        std::unique_ptr<unsigned char[]> msgPtr ( new unsigned char [nonConstMsg.length()+1]);  
        std::fill_n(msgPtr.get(), msg.length()+1, 0x00);      
        std::string::const_iterator iter = nonConstMsg.begin();
        for (unsigned int i = 0; i < nonConstMsg.size();++i){
            msgPtr.get()[i] = *iter ; ++ iter ; 
        }        
        Base58EncDec encdec;
        std::vector<uint8_t> decodedVal = encdec.decode(nonConstMsg);
        std::string retVal;
        for(std::vector<uint8_t>::const_iterator iter = decodedVal.begin();iter != decodedVal.end(); ++ iter){
            retVal.push_back(*iter);
        } 
#ifdef __EMSCRIPTEN__
        return retVal.c_str() ;         
#else        
        return retVal ; 
#endif        
    }    


    MESSAGE_HASH_RETURN_TYPE EncodeBase58Checked (const std::string& msg)
    {
        std::vector<uint8_t> vec; 
        for (std::string::const_iterator iter = msg.begin(); iter != msg.end(); ++ iter){
            vec.push_back(*iter);
        }        
        Base58EncDec encdec ; 
        std::string  encVal =  encdec.encodeCheck (vec);
#ifdef __EMSCRIPTEN__
        return encVal.c_str() ;         
#else        
        return encVal ; 
#endif        
    }

    MESSAGE_HASH_RETURN_TYPE DecodeBase58Checked (const std::string& msg)
    {       
        std::string nonConstMsg ( msg ); 
        nonConstMsg = nonConstMsg.erase(nonConstMsg.find_last_not_of("\t\n\v\f\r ")+1);
        
        
        std::unique_ptr<unsigned char> msgPtr ( new unsigned char [nonConstMsg.length()+1]);  
        std::fill_n(msgPtr.get(), msg.length()+1, 0x00);      
        std::string::const_iterator iter = nonConstMsg.begin();
        for (unsigned int i = 0; i < nonConstMsg.size();++i){
            msgPtr.get()[i] = *iter ; ++ iter ; 
        }        
        Base58EncDec encdec;
        std::vector<uint8_t> decodedVal = encdec.decodeCheck(nonConstMsg);
        std::string retVal;
        for(std::vector<uint8_t>::const_iterator iter = decodedVal.begin();iter != decodedVal.end(); ++ iter){
            retVal.push_back(*iter);
        } 
#ifdef __EMSCRIPTEN__
        return retVal.c_str() ; 
#else        
        return retVal ; 
#endif        
    }      
