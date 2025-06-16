#ifndef IModelObserverH
#define IModelObserverH

class IModelObserver {
public:
    virtual ~IModelObserver() {}
    // Model이 데이터 변경을 알리기 위해 호출하는 순수 가상 함수
    virtual void Update() = 0;
};

#endif