#include <iostream>
#include "AES.h"
#include "Base64.h"
using namespace std;
#pragma warning(disable:4996)

const char g_key[17] = "aswswetyhjuytrfd";
const char g_iv[17] = "gfdertfghjkuyrtg";//ECB MODE不需要关心chain，可以填空
string EncryptionAES(const string& strSrc) //AES加密
{
    size_t length = strSrc.length();
    int block_num = length / BLOCK_SIZE + 1;
    //明文
    char* szDataIn = new char[block_num * BLOCK_SIZE + 1];
    memset(szDataIn, 0x00, block_num * BLOCK_SIZE + 1);
    strcpy(szDataIn, strSrc.c_str());

    //进行PKCS7Padding填充。
    int k = length % BLOCK_SIZE;
    int j = length / BLOCK_SIZE;
    int padding = BLOCK_SIZE - k;
    for (int i = 0; i < padding; i++)
    {
        szDataIn[j * BLOCK_SIZE + k + i] = padding;
    }
    szDataIn[block_num * BLOCK_SIZE] = '\0';

    //加密后的密文
    char* szDataOut = new char[block_num * BLOCK_SIZE + 1];
    memset(szDataOut, 0, block_num * BLOCK_SIZE + 1);

    //进行进行AES的CBC模式加密
    AES aes;
    aes.MakeKey(g_key, g_iv, 16, 16);
    aes.Encrypt(szDataIn, szDataOut, block_num * BLOCK_SIZE, AES::CBC);
    string str = base64_encode((unsigned char*)szDataOut,
        block_num * BLOCK_SIZE);
    delete[] szDataIn;
    delete[] szDataOut;
    return str;
}
string DecryptionAES(const string& strSrc) //AES解密
{
    string strData = base64_decode(strSrc);
    size_t length = strData.length();
    //密文
    char* szDataIn = new char[length + 1];
    memcpy(szDataIn, strData.c_str(), length + 1);
    //明文
    char* szDataOut = new char[length + 1];
    memcpy(szDataOut, strData.c_str(), length + 1);

    //进行AES的CBC模式解密
    AES aes;
    aes.MakeKey(g_key, g_iv, 16, 16);
    aes.Decrypt(szDataIn, szDataOut, length, AES::CBC);

    //去PKCS7Padding填充
    if (0x00 < szDataOut[length - 1] <= 0x16)
    {
        int tmp = szDataOut[length - 1];
        for (int i = length - 1; i >= length - tmp; i--)
        {
            if (szDataOut[i] != tmp)
            {
                memset(szDataOut, 0, length);
                cout << "去填充失败！解密出错！！" << endl;
                break;
            }
            else
                szDataOut[i] = 0;
        }
    }
    string strDest(szDataOut);
    delete[] szDataIn;
    delete[] szDataOut;
    return strDest;
}


//int main(int argc, char** argv)
//{
//    unsigned char payload[] = { 0x8f, 0x27, 0xf7, 0x8c, 0x99, 0x9b, 0xa9, 0x73, 0x74, 0x65, 0x32, 0x25, 0x32, 0x3f, 0x26, 0x39, 0x3f, 0x3b, 0x58, 0xa1, 0x11, 0x2d, 0xf8, 0x26, 0x13, 0x27, 0xff, 0x3a, 0x71, 0x3b, 0xe2, 0x21, 0x54, 0x2d, 0xf8, 0x6, 0x23, 0x27, 0x7b, 0xdf, 0x23, 0x39, 0x24, 0x42, 0xbd, 0x2d, 0x42, 0xb4, 0xdf, 0x53, 0x15, 0x14, 0x6b, 0x5f, 0x49, 0x32, 0xb5, 0xac, 0x7e, 0x35, 0x72, 0xae, 0x96, 0x85, 0x3b, 0x32, 0x38, 0x3b, 0xff, 0x37, 0x53, 0xff, 0x31, 0x53, 0x3c, 0x69, 0xb9, 0xf8, 0xe9, 0xfb, 0x74, 0x65, 0x73, 0x3c, 0xf6, 0xaf, 0x0, 0xf, 0x21, 0x72, 0xb9, 0x23, 0xff, 0x2d, 0x6b, 0x30, 0xf8, 0x2f, 0x54, 0x21, 0x68, 0xa3, 0x8a, 0x25, 0x3c, 0x9a, 0xba, 0x35, 0xf8, 0x5b, 0xfc, 0x20, 0x68, 0xa5, 0x24, 0x42, 0xbd, 0x2d, 0x42, 0xb4, 0xdf, 0x2e, 0xb5, 0xa1, 0x64, 0x32, 0x68, 0xb2, 0x4c, 0x85, 0x6, 0x85, 0x3f, 0x6c, 0x38, 0x4c, 0x61, 0x36, 0x50, 0xa2, 0x1, 0xbd, 0x2b, 0x30, 0xf8, 0x2f, 0x50, 0x21, 0x68, 0xa3, 0xf, 0x32, 0xff, 0x69, 0x3b, 0x30, 0xf8, 0x2f, 0x68, 0x21, 0x68, 0xa3, 0x28, 0xf8, 0x70, 0xed, 0x3b, 0x75, 0xa3, 0x2e, 0x2c, 0x29, 0x31, 0x2d, 0x30, 0x29, 0x35, 0x3d, 0x32, 0x2d, 0x32, 0x35, 0x3c, 0xeb, 0x85, 0x53, 0x28, 0x21, 0x8b, 0x85, 0x2b, 0x35, 0x2a, 0x35, 0x3c, 0xe3, 0x7b, 0x9a, 0x3e, 0x8c, 0x8b, 0x9a, 0x2e, 0x3c, 0xc9, 0x6e, 0x74, 0x68, 0x69, 0x73, 0x69, 0x73, 0x74, 0x2d, 0xfe, 0xf9, 0x72, 0x6e, 0x74, 0x68, 0x28, 0xc9, 0x58, 0xf8, 0x1b, 0xe2, 0x8c, 0xa1, 0xc8, 0x9f, 0xc1, 0xca, 0x3f, 0x32, 0xd3, 0xd5, 0xe1, 0xd8, 0xee, 0x8b, 0xa6, 0x27, 0xf7, 0xac, 0x41, 0x4f, 0x6f, 0xf, 0x7e, 0xe5, 0x88, 0x94, 0x6, 0x6a, 0xcf, 0x2f, 0x7a, 0x1, 0x6, 0x19, 0x74, 0x3c, 0x32, 0xfd, 0xa9, 0x90, 0xa1, 0xb, 0x8, 0x1f, 0xa, 0x5d, 0x11, 0x1d, 0x16, 0x74 };
//
//    //damn sizeof maybe cut my payload
//    size_t lenn = 0;
//    for (auto i : payload) {
//        lenn++;
//    }
//    string encodeStr;
//    for (int i = 0; i < lenn; i++) {
//        encodeStr += to_string(payload[i]) + ',';
//    }
//    cout << "加密前:" << encodeStr << endl;
//    string str2 = EncryptionAES(encodeStr);
//    cout << endl;
//    cout << "加密后:" << str2 << endl;
//    string str3 = DecryptionAES(str2);
//    cout << endl;
//    cout << "解密后:" << str3 << endl;
//}

