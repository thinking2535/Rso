#pragma once

#include "Base.h"
#include <Rso/Net/Base.h>

namespace rso
{
	namespace filetransfer
	{
		using namespace net;

		template<typename _TNet>
		class CFileTransfer abstract
		{
		protected:
			unique_ptr<_TNet> _Net;

		private:
			TGetDoneFunc _GetDoneFunc;
			TPutDoneFunc _PutDoneFunc;
			TErrorFunc _ErrorFunc;

		protected:
			bool _Recv(const CKey& Key_, CStream& Stream_)
			{
                try
                {
                    bool PutDone = false;
                    Stream_ >> PutDone;

                    if (PutDone)
                    {
                        bool PutResult = false;
                        Stream_ >> PutResult;

                        wstring Dst;
                        Stream_ >> Dst;

                        if (PutResult)
                            _PutDoneFunc(Key_.PeerNum, Dst);
                        else
                            _ErrorFunc(Key_.PeerNum, Dst + L" Put Fail");
                    }
                    else
                    {
                        bool Request = false;
                        Stream_ >> Request;

                        if (Request)
                        {
                            wstring Src;
                            Stream_ >> Src;

                            wstring Dst;
                            Stream_ >> Dst;

                            Put(Key_.PeerNum, Src, Dst);
                        }
                        else
                        {
                            wstring Dst;
                            Stream_ >> Dst;

                            try
                            {
                                ofstream f(Dst, ios::binary);
                                if (!f.is_open())
                                    THROWEX();

                                f.write(Stream_.buff(), Stream_.size());
                                f.close();

                                _GetDoneFunc(Key_.PeerNum, Dst);

                                _Net->Send(Key_.PeerNum, true, true, Dst);
                            }
                            catch (...)
                            {
                                _ErrorFunc(Key_.PeerNum, L"Out FileWrite Fail");
                            }
                        }
                    }

                    Stream_.clear();
                }
                catch (...)
                {
                    _ErrorFunc(Key_.PeerNum, L"_Recv Fail");
                    return false;
                }

                return true;
			}

		public:
			CFileTransfer(_TNet* Net_, TGetDoneFunc GetDoneFunc_, TPutDoneFunc PutDoneFunc_, TErrorFunc ErrorFunc_) :
				_Net(Net_),
				_GetDoneFunc(GetDoneFunc_), _PutDoneFunc(PutDoneFunc_), _ErrorFunc(ErrorFunc_)
			{}
			void Put(TPeerCnt PeerNum_, const wstring& Src_, const wstring& Dst_)
			{
				try
				{
					CStream StreamSrc;
                    StreamSrc.LoadFile(Src_);

					_Net->Send(PeerNum_, false, false, Dst_, StreamSrc);
				}
				catch (...)
				{
					_ErrorFunc(PeerNum_, L"In FileRead Fail");
				}
			}
			inline void Get(TPeerCnt PeerNum_, const wstring& Src_, const wstring& Dst_)
			{
				_Net->Send(PeerNum_, false, true, Src_, Dst_);
			}
			inline void Proc(void)
			{
				_Net->Proc();
			}
			inline void Close(TPeerCnt PeerNum_)
			{
				_Net->Close(PeerNum_);
			}
			inline void CloseAll(void)
			{
				_Net->CloseAll();
			}
		};
	}
}
