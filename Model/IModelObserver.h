// Model/IModelObserver.h

#ifndef IModelObserverH
#define IModelObserverH

#include <System.Classes.hpp>
#include <System.SysUtils.hpp>

// [수정] VCL/COM 스타일을 따르기 위해 IUnknown 상속
class IModelObserver : public IUnknown
{
public:
    // Model의 데이터가 업데이트될 때 호출될 함수
    virtual void __fastcall onModelUpdate() = 0;

    // --- IUnknown의 기본 메소드들 ---
    // C++ Builder 델파이 스타일의 인터페이스는 참조 카운팅을 사용합니다.
    // TForm 같은 컴포넌트에 구현할 때는 간단히 0을 반환해도 무방합니다.
    virtual HRESULT __stdcall QueryInterface(const GUID&, void**) { return E_NOTIMPL; }
    virtual ULONG __stdcall AddRef() { return 0; }
    virtual ULONG __stdcall Release() { return 0; }
};

#endif