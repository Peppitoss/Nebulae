#include"Barrier.h"

namespace barrier
{

	Barrier::Barrier(int threads)
	{
		ArriveCounter = 0;
		LeaveCounter = threads;
		Flag = 0;
		pThreads = SDL_atomic_t{ threads }; // threads;
	}

	Barrier::Barrier()
	{
		ArriveCounter = 0;
		LeaveCounter = 0;
		Flag = 0;
		pThreads = SDL_atomic_t{ 0 };
	}

	void Barrier::Break() 
	{
		this->Lock();
		this->ArriveCounter = SDL_AtomicGet(&pThreads);
		this->Unlock();
	}

	Barrier::~Barrier()
	{

	}

	MutexBarrier::MutexBarrier()
		:Barrier(0)
	{
		Mutex = nullptr;
	}

	MutexBarrier::MutexBarrier(int threads)
		:Barrier(threads)
	{
		Mutex = SDL_CreateMutex();
	}

	//MutexBarrier::~MutexBarrier()
	//{
	//	SDL_UnlockMutex(Mutex);
	//	SDL_DestroyMutex(Mutex);
	//}

	SpinlockBarrier::SpinlockBarrier()
		: Barrier(0)
	{
		Spinlock = new int(0);
	}

	SpinlockBarrier::SpinlockBarrier(int threads)
		: Barrier(threads)
	{
		Spinlock = new int(0);
	}

	SpinlockBarrier::~SpinlockBarrier()
	{
		delete Spinlock;
	}

	ConditionWaitBarrier::ConditionWaitBarrier()
		: MutexBarrier(0)
	{
		pBreak = SDL_FALSE;
	}

	ConditionWaitBarrier::ConditionWaitBarrier(int threads, SDL_bool initFlag)
		: MutexBarrier(threads)
	{
		pBreak = SDL_FALSE;
		condition = initFlag;
		cond = SDL_CreateCond();
	}

	void ConditionWaitBarrier::Break()
	{
		this->Lock();
		pBreak = SDL_TRUE;
		SDL_CondBroadcast(this->sdlCond());
		this->Unlock();
	}

	void ConditionWaitBarrier::Reset()
	{
		this->Lock();
		pBreak = SDL_FALSE;
		ArriveCounter = 0;
		LeaveCounter = this->Threads();
		Flag = 0;
		this->Unlock();
	}

	void SetBarrier(Barrier* b)
	{
		b->Lock();
		if (b->LeaveCounter == b->Threads())
		{
			b->Unlock();
			while (b->LeaveCounter != b->Threads()); // wait for all to leave before clearing
				b->Lock();
		}

		if (b->ArriveCounter == 0) // no other threads in barrier
		{
			b->Flag = 0;
		}
		b->ArriveCounter++;
		int arrived = b->ArriveCounter;
		b->Unlock();

		if (arrived == b->Threads()) // last arriver sets flag
		{
			b->ArriveCounter = 0;
			b->LeaveCounter = 1;
			b->Flag = 1;
		}
		else
		{

		while (b->Flag == 0); // wait for flag
		
		b->Lock();
		b->LeaveCounter++;
		b->Unlock();
		}
	}
	void SetSRCBarrier(Barrier* b, int ZeroFlag)
	{
		int local_sense = ZeroFlag > 0 ? 1 : 0;

		b->Lock();// b->lock.lock();
		b->ArriveCounter++; //->counter++;
		int arrived = b->ArriveCounter;

		if (arrived == b->Threads()) // last arriver sets flag
		{
			b->Unlock();
			b->ArriveCounter = 0;
			// memory fence to ensure that the change to counter
			// is seen before the change to flag
			b->Flag = local_sense;
		}
		else
		{
			b->Unlock();
			while (b->Flag != local_sense); // wait for flag
		}
	}
	void SetSDLCondWaitBarrier(ConditionWaitBarrier* b, int ZeroFlag)
	{

		SDL_bool local_sense = ZeroFlag > 0 ? SDL_TRUE : SDL_FALSE;

		b->Lock();// b->lock.lock();
		b->ArriveCounter++; //->counter++;
		int arrived = b->ArriveCounter;

		if (arrived == b->Threads()) // last arriver sets flag
		{
			b->ArriveCounter = 0;
			b->SetCondition(local_sense);
			SDL_CondBroadcast(b->sdlCond());
			b->Unlock();
		}
		else
		{
			while (b->GetBreak() == SDL_FALSE && (b->Condition() != local_sense))
				SDL_CondWait(b->sdlCond(), b->sdlLock());
			b->Unlock();
		}
	}

	//ConditionWaitBarrier::~ConditionWaitBarrier()
	//{
	//	SDL_DestroyCond(cond);
	//	//SDL_DestroyMutex(Mutex);
	//}

}