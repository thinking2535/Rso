#include "Mail.h"


namespace rso
{
	namespace mail
	{
		CMail::CMail()
		{
		}

		CMail::~CMail()
		{
		}

		void	CMail::Send(TAddress From_, TAddress To_, TTitle Title_, TBody Body_)
		{
			CoInitialize(NULL);
			{
				IMessagePtr iMsg(__uuidof(Message));
				FieldsPtr   Flds;
				_StreamPtr  pStm;

				iMsg->To = To_.c_str();
				iMsg->From = From_.c_str();
				iMsg->Subject = Title_.c_str();

				// Now set some custom headers for the message
				Flds = iMsg->Fields;
				Flds->Item["urn:schemas:mailheader:X-Mailer"]->Value = "Microsoft CDO for Windows 2000";
				Flds->Item["urn:schemas:mailheader:Myheader"]->Value = "come value";
				Flds->Item["urn:schemas:mailheader:X-Mailer"]->Value = "Microsoft CDO for Windows 2000";
				Flds->Item["urn:schemas:mailheader:Myheader"]->Value = "come value";
				Flds->Update();

				IBodyPartPtr iBp;
				iBp = iMsg;
				IBodyPartPtr iBp2;
				Flds = iBp->Fields;
				Flds->Item["urn:schemas:mailheader:content-type"]->Value = "multipart/alternative";
				Flds->Update();

				iBp2 = iBp->AddBodyPart(-1);
				Flds = iBp2->Fields;
				Flds->Item["urn:schemas:mailheader:content-type"]->Value = "text/plain";
				Flds->Item["urn:schemas:mailheader:content-transfer-encoding"]->Value = "quoted-printable";
				Flds->Update();

				pStm = iBp2->GetDecodedContentStream();
				pStm->WriteText("This is a test", stWriteLine);
				pStm->Flush();

				iBp2 = iBp->AddBodyPart(-1);
				Flds = iBp2->Fields;
				Flds->Item["urn:schemas:mailheader:content-type"]->Value = "text/html";
				Flds->Item["urn:schemas:mailheader:content-transfer-encoding"]->Value = "quoted-printable";
				Flds->Update();

				pStm = iBp2->GetDecodedContentStream();

				pStm->WriteText(Body_.c_str(), stWriteLine);

				pStm->Flush();

				iMsg->Send();   //메일 전송
			}
			CoUninitialize();
		}
	}
}
