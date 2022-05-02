#include "stdafx.h"
#include <chrono>
#include <iostream>
#include <string>
#include <Rso/Crypto/Uri.h>
#include <Rso/Crypto/Base64.h>
#include <Rso/Crypto/AesCrypto.h>
#include <Rso/Crypto/Crypto.h>
#include <Rso/Crypto/sha256.h>
#include <Rso/Crypto/UID64.h>

using namespace std;
using namespace std::chrono;
using namespace rso;
using namespace crypto;

void Print( const unsigned char* pData_, int Size_ )
{
	for( int i=0; i<Size_; ++i )
	{
		cout.width(4);
		cout << (int)pData_[i] << " ";
	}
	cout << endl;
}

int _tmain(int argc, _TCHAR* argv[])
{
	CUID64 uid64(372319);

	for (int i = 0; i < 100; ++i)
	{
		auto e = uid64.Encode(i);
		wstring s{e.begin(), e.end()};
		wcout << s << L"  ";
		auto d = uid64.Decode(e);
		wcout << d << endl;
	}

	return 0;

#if 0	// Aes Test
	CAesCrypto c(CAesCrypto::Bit_128);
	unsigned char key[32] = {};
	c.SetKey( key );
	unsigned char ee[1024] = "Here is some data to encrypt!";
	unsigned char dd[1024] = {};

	const int c_i = 1000000;
	auto start = system_clock::now();
	for( int i=0; i<c_i; ++i )
	{
		c.Encrypt( ee, sizeof(ee), dd );
		c.Decrypt( dd, sizeof(dd), ee );
	}
	auto end = system_clock::now();
	cout << duration_cast<milliseconds>(end-start).count() << endl;
#elif 0

	CCrypto cr;


	//string s("slkdfjlskjfwoeifjlskdjf");
	//string s2("slkdfjlskjfwoeifjlskdjf");

	unsigned char s[] = { 9,8,7,6,5,4,3,2,1,0 };

	const int c_i = 10;
	auto start = system_clock::now();
	for( int i=0; i<c_i; ++i )
	{
		cr.Encode(s, sizeof(s), 0x1f3a49b72c8d5ef6);
		Print( s, sizeof(s) );
		cr.Decode(s, sizeof(s), 0x1f3a49b72c8d5ef6);
		Print( s, sizeof(s) );

		//if( s2 != s )
		//{
		//	cout << "error" << endl;
		//	return 0;
		//}
	}
	auto end = system_clock::now();
	cout << duration_cast<milliseconds>(end-start).count() << endl;

#elif 1

	sha256 cr;

	cout << cr.to_string("") << endl;

#endif



	//string s("ÇÏÀÌzz~!@");
	//string e(CUri::Encode(s));
	//string d(CUri::Decode(e));

	//printf( "%s\n", s.c_str() );
	//printf( "%s\n", e.c_str() );
	//printf( "%s\n", d.c_str() );
	//cout << endl;

	//"Pz8/aGl+IEB+fiFAIyQlXiYqKCkrX1FXRVJUWVVJT1AJPz8/Pz8/Pz8/Pz8="
	
//	char p[] = "Man[2] is distinguished, not only by his reason, but by this singular passion from other animals, which is a lust of the mind, that by a perseverance of delight in the continued and indefatigable generation of knowledge, exceeds the int16 vehemence of any carnal pleasure.";
	//string e = CBase64::Encode( string("A") );
	//cout << e << endl;
	//cout << CBase64::Decode( e ) << endl;


	//CAesCrypto e(CAesCrypto::Bit_192);

	//system_clock::time_point	s;
	//system_clock::time_point	en;

	//const int c_i = 1000;
	//unsigned char ee[1024] = "Here is some data to encrypt!";
	//unsigned char dd[1024] = "";
	//unsigned char tt[1024] = "";
	//unsigned char key[32] = "asdf";

	//e.SetKey( key );

	//s = system_clock::now();
	//for( int j=0; j<c_i; ++j )
	//{
	//	e.Encrypt( ee, sizeof(ee), tt );
	//	e.Decrypt( tt, sizeof(ee), dd );
	//}
	//en = system_clock::now();
	//cout << duration_cast<milliseconds>(en-s).count() << endl;

	//for( int i=0; i<16; ++i )
	//{
	//	if( dd[i] != ee[i] )
	//	{
	//		printf( "error" );
	//		return 0;
	//	}
	//}

	//printf( "ok" );

	return 0;
}

