//#include"IntegrationFunction.h"
//
//
//
//template <typename T>
//IntegrationFunction<T>::IntegrationFunction(T(*func) (const T, Millisecond))
//{
//	pFunc = func;
//}
//
//template <typename T>
//T IntegrationFunction<T>::RunFunction(const T in, Millisecond time)
//{
//	return pFunc(in, time);
//}
//

//template <class T>
//LinearRateOfChange<T>::LinearRateOfChange(double changePrSecond)
//{
//	pChangePrSecond = changePrSecond;
//}
//
//template <typename T>
//T LinearRateOfChange<T>::RunFunction(const T in, Millisecond time)
//{
//	return in + pChangePrSecond*time.GetValueAsSeconds();
//}
