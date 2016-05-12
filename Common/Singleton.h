#ifndef _SINGLETON_H_
#define _SINGLETON_H_

template <typename T> class CSingleton
{
	// The one and only instance of this class
	static T * ms_Singleton;

	public:
		// Sets up the one and only instance
		CSingleton()
		{
			int offset = (int)(T*)1 - (int)(CSingleton<T>*)(T*)1;
			ms_Singleton = (T*)((int)this + offset);
		}

		// Destroys the one and only instance
		~CSingleton()
		{
			ms_Singleton = 0;
		}

		// Returns the singleton
		static T& GetSingleton()
		{
			return (*ms_Singleton);
		}

		// Returns a pointer to the singleton
		static T* GetSingletonPtr()
		{
			return ms_Singleton;
		}
};

// This might look a bit confusing, but all it does is for each new singleton type that is created
// it initialises it's static T * ms_Singleton member.
template <typename T> T * CSingleton <T>::ms_Singleton = 0;

#endif