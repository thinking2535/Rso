#pragma once

#include <string>
#include <array>
#include <list>
#include <vector>
#include <set>
#include <map>
#include <Rso/Core/Stream.h>
#include <Rso/Json/Json.h>

#include <Rso/Physics/Protocol.h>
namespace rso
{
	namespace unity
	{
		using namespace core;
		using namespace json;
		using namespace physics;
		struct SBoxCollider2D : public STransform
		{
			SRectCollider2D RectCollider2D{};
			SBoxCollider2D()
			{
			}
			SBoxCollider2D(const STransform& Super_, const SRectCollider2D& RectCollider2D_) : STransform(Super_), RectCollider2D(RectCollider2D_)
			{
			}
			SBoxCollider2D(STransform&& Super_, SRectCollider2D&& RectCollider2D_) : STransform(std::move(Super_)), RectCollider2D(std::move(RectCollider2D_))
			{
			}
			void operator << (CStream& Stream_) override
			{
				STransform::operator << (Stream_);
				Stream_ >> RectCollider2D;
			}
			void operator << (const Value& Value_) override
			{
				STransform::operator << (Value_);
				Value_["RectCollider2D"] >> RectCollider2D;
			}
			void operator >> (CStream& Stream_) const override
			{
				STransform::operator >> (Stream_);
				Stream_ << RectCollider2D;
			}
			void operator >> (Value& Value_) const override
			{
				STransform::operator >> (Value_);
				Value_["RectCollider2D"] = RectCollider2D;
			}
			static wstring StdName(void)
			{
				return 
					GetStdName(STransform()) + L"," + 
					GetStdName(SRectCollider2D());
			}
			static wstring MemberName(void)
			{
				return 
					GetMemberName(STransform(), L"") + L"," + 
					GetMemberName(SRectCollider2D(), L"RectCollider2D");
			}
		};
	}
}
