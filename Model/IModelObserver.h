// Model/IModelObserver.h

#ifndef IModelObserverH
#define IModelObserverH

// Model의 상태 변경을 감지할 Observer(관찰자)를 위한 인터페이스입니다.
// View가 이 인터페이스를 상속받아 Model의 변경을 통지받습니다.
class IModelObserver {
public:
    // Model의 데이터가 업데이트될 때 호출될 순수 가상 함수
    virtual void onModelUpdate() = 0;
    // 가상 소멸자
    virtual ~IModelObserver() {}
};

#endif