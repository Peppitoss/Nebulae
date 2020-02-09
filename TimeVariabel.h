//#pragma once
//#include <memory>
//#include"TimeUnit.h"
//#include"IntegrationFunction.h"
//
////class ColorRGB;
//
//template <class  T>
//class TimeVariabel
//{
//
//public:
//
//	TimeVariabel();
//	TimeVariabel(const TimeVariabel<T> & other);
//	TimeVariabel(const TimeVariabel<T> && other);
//
//	TimeVariabel(T);
//	TimeVariabel(T, T(*func) (const T, Millisecond));
//	TimeVariabel(T, IntegrationFunction<T>* function);
//	TimeVariabel(IntegrationFunction<T>* function);
//	
//
//	TimeVariabel(TimeVariabel<T> & other) { this->pValue = other.pValue; this->pInternalFunction = other.pInternalFunction; }
//	TimeVariabel(TimeVariabel<T> && other) { this->pValue = std::move(other.pValue); this->pInternalFunction = std::move(other.pInternalFunction); }
//
//	void ResetFunction(IntegrationFunction<T>* function);
//	void ResetFunction(T(*func) (const T, Millisecond));
//	void UpdateVariabel(Millisecond time);
//
//	bool HasFunction() const { return this->pInternalFunction.get(); }
//
//	TimeVariabel<T>& operator=(const TimeVariabel<T>& other) { this->pValue = other.pValue; this->pInternalFunction = other.pInternalFunction; return *this; };
//	TimeVariabel<T>& operator=(const TimeVariabel<T>&& other) { this->pValue = std::move(other.pValue); this->pInternalFunction = std::move(other.pInternalFunction); return *this; };
//
//	TimeVariabel<T>& operator=(const T& other) { this->pValue = other;  return *this;};
//	TimeVariabel<T>& operator=(const T&& other) { this->pValue = std::move(other);  return *this;};
//
//	void ChangeVariabelToValue(T x) { pValue = x; }
//	T GetVariabel() const { return pValue; }
//
//private:
//
//	//Millisecond pAccumulatedTime;
//	T pValue;
//	std::shared_ptr<IntegrationFunction<T>> pInternalFunction;
//
//};
////template <class  T>
////T operator=(const TimeVariabel<T>& other) { return other.pValue; };
//
//template class TimeVariabel<ColorRGB>;
//template class TimeVariabel<int>;
//template class TimeVariabel<double>;