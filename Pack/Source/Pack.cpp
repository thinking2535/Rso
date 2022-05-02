#include "Pack.h"

NS_BEGIN

namespace pack
{
	ERet			CPack::ResizeFile(UFileSize Size_)
	{
		if (m_hFile == INVALID_HANDLE_VALUE) return Ret_InvalidHandle;

		if (SetFilePointer(m_hFile, Size_.Low, (PLONG)&Size_.High, FILE_BEGIN) == INVALID_SET_FILE_POINTER)
			return Ret_SetFilePointerFail;

		if (SetEndOfFile(m_hFile) == FALSE) return Ret_SetEOFFail;

		return Ret_Ok;
	}

	ERet			CPack::LoadIndexPtr(void)
	{
		DWORD		dwReadSize = 0;

		if (m_hFile == INVALID_HANDLE_VALUE) return Ret_InvalidHandle;

		LONG High = 0;
		if (SetFilePointer(m_hFile, 0, &High, FILE_BEGIN) == INVALID_SET_FILE_POINTER) return Ret_SetFilePointerFail;

		if (ReadFile(m_hFile, m_IndexPtr.GetDataPtr(), m_IndexPtr.GetDataSize(), &dwReadSize, NULL) == FALSE) return Ret_ReadFileFail;

		if (dwReadSize != m_IndexPtr.GetDataSize()) return Ret_ReadSizeFail;

		if (m_IndexPtr.Get().Size == c_FileSize_Max) return Ret_BrokenIndexPtr;

		return Ret_Ok;
	}

	ERet			CPack::LoadIndex(void)
	{
		ERet	Ret = Ret_Ok;
		CHAR*	pIndexData = NULL;

		if (m_hFile == INVALID_HANDLE_VALUE)
		{
			Ret = Ret_InvalidHandle;
			goto LABEL_ERROR;
		}

		Ret = LoadIndexPtr();
		if (Ret != Ret_Ok) goto LABEL_ERROR;


		{
			UFileSize	IndexPtr = m_IndexPtr.Get();
			if (SetFilePointer(m_hFile, IndexPtr.Low, (PLONG)&IndexPtr.High, FILE_BEGIN) == INVALID_SET_FILE_POINTER)
			{
				Ret = Ret_SetFilePointerFail;
				goto LABEL_ERROR;
			}
		}


		{
			SIndexHeader	IndexHeader;
			DWORD dwReadSize = 0;
			if (ReadFile(m_hFile, &IndexHeader, sizeof(IndexHeader), &dwReadSize, NULL) == FALSE)
			{
				Ret = Ret_ReadFileFail;
				goto LABEL_ERROR;
			}
			if (dwReadSize != sizeof(IndexHeader))
			{
				Ret = Ret_InvalidReadSize;
				goto LABEL_ERROR;
			}

			if (IndexHeader.Size == 0) return Ret_Ok;

			if (IndexHeader.Size < 0)
			{
				Ret = Ret_InvalidIndexSize;
				goto LABEL_ERROR;
			}

			pIndexData = new(nothrow)CHAR[IndexHeader.Size];
			if (pIndexData == NULL)
			{
				Ret = Ret_NotEnoughMemory;
				goto LABEL_ERROR;
			}


			if (ReadFile(m_hFile, pIndexData, IndexHeader.Size, &dwReadSize, NULL) == FALSE)
			{
				Ret = Ret_ReadFileFail;
				goto LABEL_ERROR;
			}
			if (dwReadSize != IndexHeader.Size)
			{
				Ret = Ret_ReadSizeFail;
				goto LABEL_ERROR;
			}

			m_BF.Decrypt(pIndexData, IndexHeader.Size);

			if (IndexHeader.CheckSum != GetCheckSum(pIndexData, IndexHeader.Size))
			{
				Ret = Ret_InvalidCheckSum;
				goto LABEL_ERROR;
			}


			ERet DRet = m_IndexData.ReadData(pIndexData, IndexHeader.Size);
			if (DRet != Ret_Ok)
			{
				Ret = Ret_IndexDataReadFail;
				goto LABEL_ERROR;
			}
		}

		if (pIndexData != NULL) delete[] pIndexData;

		return Ret;

	LABEL_ERROR:

		if (pIndexData != NULL) delete[] pIndexData;

		return Ret;
	}

	ERet		CPack::SaveIndexPtr(UFileSize Ptr_)
	{
		ERet	Ret = Ret_Ok;

		UFileSize IndexPtrPtr(0);
		if (SetFilePointer(m_hFile, IndexPtrPtr.Low, (PLONG)&IndexPtrPtr.High, FILE_BEGIN) == INVALID_SET_FILE_POINTER)
		{
			Ret = Ret_SetFilePointerFail;
			goto LABEL_ERROR;
		}

		m_IndexPtr.Set(Ptr_.Size);
		DWORD dwWrittenSize = 0;
		if (WriteFile(m_hFile, m_IndexPtr.GetDataPtr(), m_IndexPtr.GetDataSize(), &dwWrittenSize, NULL) == FALSE)
		{
			Ret = Ret_WriteIndexFail;
			goto LABEL_ERROR;
		}
		if (dwWrittenSize != m_IndexPtr.GetDataSize())
		{
			Ret = Ret_WriteIndexSizeFail;
			goto LABEL_ERROR;
		}

		return Ret;

	LABEL_ERROR:

		return Ret;
	}

	ERet		CPack::SaveIndex(UFileSize Ptr_, CHAR* pData_, DWORD Size_)
	{
		ERet	Ret = Ret_Ok;
		CHAR*	pEncBuff = NULL;


		if (m_hFile == INVALID_HANDLE_VALUE)
		{
			Ret = Ret_InvalidHandle;
			goto LABEL_ERROR;
		}


		if (SetFilePointer(m_hFile, Ptr_.Low, (PLONG)&Ptr_.High, FILE_BEGIN) == INVALID_SET_FILE_POINTER)
		{
			Ret = Ret_SetFilePointerFail;
			goto LABEL_ERROR;
		}


		DWORD dwWrittenSize = 0;
		{
			SIndexHeader	IndexHeader;
			IndexHeader.Size = Size_;
			IndexHeader.CheckSum = GetCheckSum(pData_, Size_);

			if (Size_ + sizeof(IndexHeader) <= Size_)
			{
				Ret = Ret_IndexSizeOverFlow;
				goto LABEL_ERROR;
			}

			if (WriteFile(m_hFile, &IndexHeader, sizeof(IndexHeader), &dwWrittenSize, NULL) == FALSE)
			{
				Ret = Ret_WriteIndexHeaderFail;
				goto LABEL_ERROR;
			}
			if (dwWrittenSize != sizeof(IndexHeader))
			{
				Ret = Ret_WriteIndexHeaderSizeFail;
				goto LABEL_ERROR;
			}
		}


		pEncBuff = new(nothrow) CHAR[Size_];
		if (pEncBuff == NULL)
		{
			Ret = Ret_NotEnoughMemory;
			goto LABEL_ERROR;
		}
		CopyMemory(pEncBuff, pData_, Size_);

		m_BF.Encrypt(pEncBuff, Size_);

		if (WriteFile(m_hFile, pEncBuff, Size_, &dwWrittenSize, NULL) == FALSE)
		{
			Ret = Ret_WriteIndexFail;
			goto LABEL_ERROR;
		}
		if (dwWrittenSize != Size_)
		{
			Ret = Ret_WriteIndexSizeFail;
			goto LABEL_ERROR;
		}

		if (pEncBuff != NULL) delete[] pEncBuff;

		return Ret;

	LABEL_ERROR:

		if (pEncBuff != NULL) delete[] pEncBuff;

		return Ret;
	}

	ERet			CPack::FixIndex(void)
	{
		ERet	Ret = Ret_Ok;

		TFileSize	DataOccupySize = m_IndexData.GetDataOccupySize();
		UFileSize	FixedIndexPtr(m_IndexPtr.GetDataSize() + DataOccupySize);
		UFileSize	CurIndexPtr = m_IndexPtr.Get();


		// 교정될 인덱스포인터와 현 인덱스포인터사이의 공간이 0보다 작은가?
		if (CurIndexPtr.Size - FixedIndexPtr.Size == 0)
		{
			// Pass
		}
		else if (CurIndexPtr.Size - FixedIndexPtr.Size < 0)
		{
			// 에러
			Ret = Ret_InvalidIndexPtr;
			goto LABEL_ERROR;
		}
		// 교정될 인덱스포인터와 현 인덱스포인터사이의 공간이 인덱스 크기보다 작은가?
		else if (CurIndexPtr.Size - FixedIndexPtr.Size < sizeof(SIndexHeader) + m_IndexData.GetDataSize())
		{
			// 현 인덱스 끝으로 인덱스 새로 저장
			Ret = SaveIndex(CurIndexPtr.Size + sizeof(SIndexHeader) + m_IndexData.GetDataSize(), m_IndexData.GetDataPtr(), m_IndexData.GetDataSize());
			if (Ret != Ret_Ok) goto LABEL_ERROR;

			Ret = SaveIndexPtr(CurIndexPtr.Size + sizeof(SIndexHeader) + m_IndexData.GetDataSize());
			if (Ret != Ret_Ok) goto LABEL_ERROR;

			// 데이터 끝으로 인덱스 새로 저장
			Ret = SaveIndex(FixedIndexPtr, m_IndexData.GetDataPtr(), m_IndexData.GetDataSize());
			if (Ret != Ret_Ok) goto LABEL_ERROR;

			Ret = SaveIndexPtr(FixedIndexPtr);
			if (Ret != Ret_Ok) goto LABEL_ERROR;
		}
		// 교정될 인덱스포인터와 현 인덱스포인터사이의 공간이 인덱스 크기보다 크거나 같은가?
		else if (CurIndexPtr.Size - FixedIndexPtr.Size >= sizeof(SIndexHeader) + m_IndexData.GetDataSize())
		{
			// 데이터 끝으로 인덱스 새로 저장
			Ret = SaveIndex(FixedIndexPtr, m_IndexData.GetDataPtr(), m_IndexData.GetDataSize());
			if (Ret != Ret_Ok) goto LABEL_ERROR;

			Ret = SaveIndexPtr(FixedIndexPtr);
			if (Ret != Ret_Ok) goto LABEL_ERROR;
		}


		{
			// 파일 크기 재 조정
			UFileSize	FixedFileSize(m_IndexPtr.GetDataSize() + DataOccupySize + sizeof(SIndexHeader) + m_IndexData.GetDataSize());
			Ret = ResizeFile(FixedFileSize);
			if (Ret != Ret_Ok) goto LABEL_ERROR;
		}


		return Ret;

	LABEL_ERROR:

		return Ret;
	}


	/************************************************************************/
	/* PUBLIC FUNCTIONS                                                     */
	/************************************************************************/
	CPack::CPack() : m_hFile(INVALID_HANDLE_VALUE), m_BF(CryptType_BlowFish)
	{
	}

	CPack::~CPack()
	{
	}

	ERet		CPack::Create(const TFilePath& FilePath_)
	{
		ERet	Ret = Ret_Ok;

		ERet DRet = m_IndexData.Initialize();
		if (DRet != Ret_Ok)
		{
			Ret = Ret_IndexDataInitFail;
			goto LABEL_ERROR;
		}

		if (m_hFile != INVALID_HANDLE_VALUE)
		{
			return Ret_FileAlreadyOpened;	// 열려있었으므로 닫지 않는다.
		}

		m_hFile = CreateFile(
			FilePath_.GetPtr(),
			GENERIC_WRITE | GENERIC_READ,
			FILE_SHARE_READ, //FILE_SHARE_READ,
			NULL,
			CREATE_NEW,
			FILE_ATTRIBUTE_NORMAL,
			NULL);

		if (m_hFile == INVALID_HANDLE_VALUE)
		{
			Ret = Ret_CreateFileFail;
			goto LABEL_ERROR;
		}


		{
			UFileSize	IndexPtr(m_IndexPtr.GetDataSize());
			if (SaveIndexPtr(IndexPtr) != Ret_Ok)
			{
				Ret = Ret_WriteIndexPtrFail;
				goto LABEL_ERROR;
			}

			Ret = SaveIndex(IndexPtr, m_IndexData.GetDataPtr(), m_IndexData.GetDataSize());
			if (Ret != Ret_Ok) goto LABEL_ERROR;
		}

		return Ret;

	LABEL_ERROR:

		Close();

		return Ret;
	}

	ERet		CPack::Open(const TFilePath& FilePath_)
	{
		ERet	Ret = Ret_Ok;

		ERet DRet = m_IndexData.Initialize();
		if (DRet != Ret_Ok)
		{
			Ret = Ret_IndexDataInitFail;
			goto LABEL_ERROR;
		}


		if (m_hFile != INVALID_HANDLE_VALUE)
		{
			return Ret_FileAlreadyOpened;	// 열려있으므로 닫지 않는다.
		}

		m_hFile = CreateFile(
			FilePath_.GetPtr(),
			GENERIC_WRITE | GENERIC_READ,
			FILE_SHARE_READ, //FILE_SHARE_READ,
			NULL,
			OPEN_EXISTING,
			FILE_ATTRIBUTE_NORMAL,
			NULL);

		if (m_hFile == INVALID_HANDLE_VALUE)
		{
			Ret = Ret_OpenFileFail;
			goto LABEL_ERROR;
		}


		Ret = LoadIndex();
		if (Ret != Ret_Ok) goto LABEL_ERROR;


		Ret = FixIndex();
		if (Ret != Ret_Ok) goto LABEL_ERROR;


		return Ret;

	LABEL_ERROR:

		Close();

		return Ret;
	}

	void				CPack::Close(void)
	{
		if (m_hFile != INVALID_HANDLE_VALUE)
		{
			CloseHandle(m_hFile);
			m_hFile = INVALID_HANDLE_VALUE;
		}

		m_IndexData.Finalize();
	}

	ERet		CPack::Add(const TFilePath& FilePath_, CHAR* pData_, TFileSize DataSize_)
	{
		ERet		Ret = Ret_Ok;

		TFileSize	OriIndexPtr = m_IndexPtr.Get().Size;
		DWORD		OriIndexDataSize = m_IndexData.GetDataSize();
		CHAR*		pOriDataPtr = new(nothrow) CHAR[OriIndexDataSize];
		CHAR*		pEncBuff = NULL;
		if (pOriDataPtr == NULL)
		{
			Ret = Ret_NotEnoughMemory;
			goto LABEL_ERROR;
		}

		CopyMemory(pOriDataPtr, m_IndexData.GetDataPtr(), m_IndexData.GetDataSize());

		TFileSize	OverSize = 0;
		ERet DRet = m_IndexData.Add(FilePath_, pData_, DataSize_, OverSize);
		if (DRet != Ret_Ok)
		{
			Ret = Ret_IndexDataAddFail;
			goto LABEL_ERROR;
		}


		DWORD dwWrittenSize = 0;
		{
			UFileSize	TmpIndexPtr(OriIndexPtr + sizeof(SIndexHeader) + m_IndexData.GetDataSize() + OverSize);

			Ret = SaveIndex(TmpIndexPtr, pOriDataPtr, OriIndexDataSize);
			if (Ret != Ret_Ok) goto LABEL_ERROR_AFTERADD;

			Ret = SaveIndexPtr(TmpIndexPtr);
			if (Ret != Ret_Ok) goto LABEL_ERROR_AFTERADD;
		}


		SFile* pAddedFile = m_IndexData.GetFile(FilePath_);
		if (pAddedFile == NULL)
		{
			Ret = Ret_GetFileFail;
			goto LABEL_ERROR_AFTERADD;
		}


		// Save File Data
		TFileSize	WrittenSize = 0;
		for (TBlockCnt cnt = 0; cnt < pAddedFile->Cnt; ++cnt)
		{
			if (
				SetFilePointer(m_hFile, pAddedFile->pData[cnt].Ptr.Low + m_IndexPtr.GetDataSize(),
				(PLONG)&pAddedFile->pData[cnt].Ptr.High,
					FILE_BEGIN) == INVALID_SET_FILE_POINTER)
			{
				Ret = Ret_SetFilePointerFail;
				goto LABEL_ERROR_AFTERADD;
			}

			for (DWORD High = 0; High <= pAddedFile->pData[cnt].Size.High; ++High)
			{
				// Encrypt
				if (pEncBuff != NULL) delete[] pEncBuff;
				pEncBuff = new(nothrow) CHAR[pAddedFile->pData[cnt].Size.Low];
				if (pEncBuff == NULL)
				{
					Ret = Ret_NotEnoughMemory;
					goto LABEL_ERROR;
				}

				CopyMemory(pEncBuff, &pData_[WrittenSize], pAddedFile->pData[cnt].Size.Low);
				m_BF.Encrypt(pEncBuff, pAddedFile->pData[cnt].Size.Low);

				if (WriteFile(m_hFile, pEncBuff, pAddedFile->pData[cnt].Size.Low, &dwWrittenSize, NULL) == FALSE)
				{
					Ret = Ret_WriteIndexFail;
					goto LABEL_ERROR_AFTERADD;
				}
				if (dwWrittenSize != pAddedFile->pData[cnt].Size.Low)
				{
					Ret = Ret_WriteIndexSizeFail;
					goto LABEL_ERROR_AFTERADD;
				}
			}
		}


		// Save New Index
		{
			UFileSize	NewIndexPtr(OriIndexPtr + OverSize);

			Ret = SaveIndex(NewIndexPtr, m_IndexData.GetDataPtr(), m_IndexData.GetDataSize());
			if (Ret != Ret_Ok) goto LABEL_ERROR_AFTERADD;

			Ret = SaveIndexPtr(NewIndexPtr);
			if (Ret != Ret_Ok) goto LABEL_ERROR_AFTERADD;
		}


		Ret = FixIndex();
		if (Ret != Ret_Ok) goto LABEL_ERROR_AFTERADD;


		if (pEncBuff != NULL) delete[] pEncBuff;
		if (pOriDataPtr != NULL) delete[] pOriDataPtr;

		return Ret;


	LABEL_ERROR_AFTERADD:

		m_IndexData.Del(FilePath_);

	LABEL_ERROR:

		if (pEncBuff != NULL) delete[] pEncBuff;
		if (pOriDataPtr != NULL) delete[] pOriDataPtr;

		return Ret;
	}

	ERet		CPack::Del(const TFilePath& FilePath_)
	{
		ERet		Ret = Ret_Ok;

		TFileSize	OriIndexPtr = m_IndexPtr.Get().Size;
		TFileSize	OriIndexDataSize = m_IndexData.GetDataSize();

		ERet DRet = m_IndexData.Del(FilePath_);
		if (DRet != Ret_Ok)
		{
			Ret = Ret_IndexDataDelFail;
			goto LABEL_ERROR;
		}

		{
			UFileSize	TmpIndexPtr(OriIndexPtr + sizeof(SIndexHeader) + OriIndexDataSize);

			Ret = SaveIndex(TmpIndexPtr, m_IndexData.GetDataPtr(), m_IndexData.GetDataSize());
			if (Ret != Ret_Ok) goto LABEL_ERROR;

			Ret = SaveIndexPtr(TmpIndexPtr);
			if (Ret != Ret_Ok) goto LABEL_ERROR;
		}

		Ret = FixIndex();
		if (Ret != Ret_Ok) goto LABEL_ERROR;

		return Ret;

	LABEL_ERROR:

		m_IndexData.Del(FilePath_);

		return Ret;
	}

	TFileSize			CPack::GetFileSize(const TFilePath& FilePath_)
	{
		SFile* pFile = m_IndexData.GetFile(FilePath_);
		if (pFile == NULL) return 0;

		TFileSize	Size = 0;
		for (TBlockCnt cnt = 0; cnt < pFile->Cnt; ++cnt)
		{
			Size += pFile->pData[cnt].Size.Size;
		}

		return Size;
	}

	ERet			CPack::GetFileData(const TFilePath& FilePath_, TFileSize Size_, TFileSize& rSize_o, CHAR* pData_o)
	{
		ERet	Ret = Ret_Ok;
		SFile* pFile = m_IndexData.GetFile(FilePath_);
		if (pFile == NULL)
		{
			Ret = Ret_CantFindFile;
			goto LABEL_ERROR;
		}

		TFileSize	ReadSize = 0;
		DWORD		dwReadSize = 0;


		{
			UFileSize	BlockPtr;

			for (TBlockCnt cnt = 0; cnt < pFile->Cnt; ++cnt)
			{
				if (pFile->pData[cnt].Size.Size > Size_)
				{
					Ret = Ret_NotEnoughBufSize;
					goto LABEL_ERROR;
				}

				BlockPtr.Size = m_IndexPtr.GetDataSize() + pFile->pData[cnt].Ptr.Size;
				if (SetFilePointer(m_hFile, BlockPtr.Low, (PLONG)&BlockPtr.High, FILE_BEGIN) == INVALID_SET_FILE_POINTER)
				{
					Ret = Ret_SetFilePointerFail;
					goto LABEL_ERROR;
				}


				for (DWORD High = 0; High <= pFile->pData[cnt].Size.High; ++High)
				{
					if (ReadFile(m_hFile, &pData_o[ReadSize], pFile->pData[cnt].Size.Low, &dwReadSize, NULL) == FALSE)
					{
						Ret = Ret_ReadFileFail;
						goto LABEL_ERROR;
					}

					m_BF.Decrypt(&pData_o[ReadSize], pFile->pData[cnt].Size.Low);

					if (pFile->pData[cnt].Size.Low != dwReadSize)
					{
						Ret = Ret_ReadSizeFail;
						goto LABEL_ERROR;
					}
				}

				Size_ -= pFile->pData[cnt].Size.Size;

				ReadSize += pFile->pData[cnt].Size.Size;

				if (Size_ == 0)
				{
					if (cnt + 1 < pFile->Cnt)
					{
						Ret = Ret_NotEnoughBufSize;
						goto LABEL_ERROR;
					}

					break;
				}
			}
		}

		// Check CheckSum
		if (pFile->CheckSum != GetCheckSum(pData_o, ReadSize))
		{
			Ret = Ret_InvalidCheckSum;
			goto LABEL_ERROR;
		}

		rSize_o = ReadSize;


		return Ret;

	LABEL_ERROR:

		return Ret;
	}

	TCheckSum		CPack::GetAllFileCheckSum(void)
	{
		TCheckSum	CheckSum = 0;

		for (TDataMapIt it = m_IndexData.Begin();
			it != m_IndexData.End();
			++it)
		{
			CheckSum += (it->second).CheckSum;
		}

		return CheckSum;
	}

	void				CPack::Optimize(void)
	{
		// Defrag
	}
};

NS_END
