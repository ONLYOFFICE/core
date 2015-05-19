#pragma once

template<class T>
class SharedPtr {
private:
	struct Core {
		T* _value;
		long _counter;

		Core(T* value, const long& counter) 
			: _value(value)
			, _counter(counter) {}
	};

private:
	Core* _core;

public:
	SharedPtr(T* value = NULL) {
		_core = new Core(value, 1);
	}
	SharedPtr(const SharedPtr<T>& ptr) {
		copyCore(ptr);
	}
	~SharedPtr() {
		releaseCore();
	}

public:
	SharedPtr<T>& operator=(const SharedPtr<T>& ptr) {
		releaseCore();
		copyCore(ptr);

		return *this;
	}

public:
	T& operator*() {
		return *_core->_value;
	}
	T* operator->() {
		return _core->_value;
	}
	const T* operator->() const {
		return _core->_value;
	}

public:
	bool isEmpty() const {
		return _core->_value == NULL;
	}

private:
	void copyCore(const SharedPtr<T>& ptr) {
		_core = ptr._core;
		++_core->_counter;
	}
	void releaseCore() {
		if (!--_core->_counter)
			delete _core;
	}
};