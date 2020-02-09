//#include"TimeVariabel.h"
//
//template <typename T>
//TimeVariabel<T>::TimeVariabel()
//{
//	pInternalFunction = nullptr;// std::shared_ptr<IntegrationFunction<T>>(new IntegrationFunction<T>([](T x, Millisecond s) -> T {return x; }));
//}
//
//template <typename T>
//TimeVariabel<T>::TimeVariabel(const TimeVariabel<T> & other)
//{
//	this->pValue = other.pValue; 
//	this->pInternalFunction = other.pInternalFunction;
//}
//
//template <typename T>
//TimeVariabel<T>::TimeVariabel(const TimeVariabel<T> && other)
//{
//	this->pValue = std::move(other.pValue);
//	this->pInternalFunction = std::move(other.pInternalFunction);
//}
//
//template <typename T>
//TimeVariabel<T>::TimeVariabel(T value)
//{
//	this->pValue = value;
//	pInternalFunction = nullptr;//std::shared_ptr<IntegrationFunction<T>>(new IntegrationFunction<T>([](T x, Millisecond s) -> T {return x; }));
//}
//
//template <typename T>
//TimeVariabel<T>::TimeVariabel(T value, T(*func) (const T, Millisecond))
//{
//	this->pValue = value;
//	pInternalFunction = std::shared_ptr<IntegrationFunction<T>>( new IntegrationFunction<T>(func));
//}
//
//template <typename T>
//TimeVariabel<T>::TimeVariabel(T value, IntegrationFunction<T>* function)
//{
//	this->pValue = value;
//	pInternalFunction = std::shared_ptr<IntegrationFunction<T>>(function);
//}
//
//template <typename T>
//TimeVariabel<T>::TimeVariabel(IntegrationFunction<T>* function)
//{
//	pInternalFunction = std::shared_ptr<IntegrationFunction<T>>(function);
//}
//template <typename T>
//void TimeVariabel<T>::ResetFunction(IntegrationFunction<T>* function)
//{
//	pInternalFunction.reset(function);
//}
//
//template <typename T>
//void TimeVariabel<T>::ResetFunction(T(*func) (const T, Millisecond))
//{
//	pInternalFunction = std::shared_ptr<IntegrationFunction<T>>(new IntegrationFunction<T>(func));
//}
//
//template <typename T>
//void TimeVariabel<T>::UpdateVariabel(Millisecond time)
//{
//	if (pInternalFunction.get())
//	{
//		ChangeVariabelToValue((*pInternalFunction).RunFunction(GetVariabel(), time));
//	}
//}