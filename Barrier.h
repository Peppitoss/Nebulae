#pragma once
#include <SDL.h>

namespace barrier
{
	//From Wikipedia, barriers
	class Barrier
	{
	public:
		Barrier();
		Barrier(int threads);

		virtual void Lock() = 0;
		virtual void Unlock() = 0;

		virtual void Break();

		int ArriveCounter;
		int LeaveCounter;
		int Flag;
		void DecThreadAmount() { SDL_AtomicDecRef(&pThreads); }
		void IncThreadAmount() { SDL_AtomicIncRef(&pThreads); }
		void SetThreadAmount(int amount) { SDL_AtomicSet(&pThreads, amount); }
		int Threads() { return SDL_AtomicGet(&pThreads); }

	
		~Barrier();

	private:
		SDL_atomic_t pThreads;
	};
	class MutexBarrier : public Barrier
	{
	public:

		MutexBarrier();
		MutexBarrier(int threads);

		void Lock() { SDL_LockMutex(Mutex); }
		void Unlock() { SDL_UnlockMutex(Mutex); }

		//~MutexBarrier();

	protected:
		SDL_mutex* Mutex;

	};

	class SpinlockBarrier : public Barrier
	{
	public:

		SpinlockBarrier();
		SpinlockBarrier(int threads);

		void Lock() { SDL_AtomicLock(Spinlock); }
		void Unlock() { SDL_AtomicUnlock(Spinlock); }

		~SpinlockBarrier();

	private:
		SDL_SpinLock* Spinlock;

	};

	class ConditionWaitBarrier : public MutexBarrier
	{
	public:
		ConditionWaitBarrier();
		ConditionWaitBarrier( int threads, SDL_bool initFlag);

		void Break() override;

		SDL_cond* sdlCond() const { return cond; }
		SDL_mutex* sdlLock() const { return Mutex; }
		SDL_bool Condition() const { return condition; }
		SDL_bool GetBreak() const { return pBreak; }
		void SetBreak(SDL_bool set) { this->pBreak = set; }
		void SetCondition(SDL_bool set) { condition = set; }

		void Reset();
		//~ConditionWaitBarrier();

	private:
		SDL_bool pBreak;
		SDL_bool condition;
		SDL_cond *cond;
	};

	//From Wikipedia, barriers
	void SetBarrier(Barrier* barrier);
	void SetSRCBarrier(Barrier* barrier, int ZeroFlag);
	void SetSDLCondWaitBarrier(ConditionWaitBarrier* barrier, int ZeroFlag);
	//void SetSDLCondWaitBroadcast(ConditionWaitBarrier* barrier);

}