#include "Parser.h"

namespace rso
{
    namespace proto
    {
        wstring CParser::Get(void)
        {
            if (_Index >= _Str.size())
                return wstring();

            wstring::size_type OldIndex = 0;
            for (auto Pos = _Index; Pos < _Str.size(); ++Pos)
            {
                if (_Str[Pos] == L'{')
                {
                    if (Pos == _Index)
                    {
                        ++_Index;
                        return L"{";
                    }

                    OldIndex = _Index;
                    _Index = Pos;
                    return _Str.substr(OldIndex, Pos - OldIndex);
                }
                else if (_Str[Pos] == L'}')
                {
                    if (Pos == _Index)
                    {
                        ++_Index;
                        return L"}";
                    }

                    OldIndex = _Index;
                    _Index = Pos;
                    return _Str.substr(OldIndex, Pos - OldIndex);
                }
                else if (_Str[Pos] == L',')
                {
                    if (Pos == _Index)
                    {
                        ++_Index;
                        continue;
                    }

                    OldIndex = _Index;
                    _Index = Pos;
                    return _Str.substr(OldIndex, Pos - OldIndex);
                }
            }

            OldIndex = _Index;
            _Index = _Str.size();
            return _Str.substr(OldIndex);
        }
    }
}
