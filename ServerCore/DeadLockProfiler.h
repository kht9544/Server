#pragma once
class DeadLockProfiler
{
public:
	void PushLock(const char* name);
	void PopLock(const char* name);
	void CheckCycle();

private:
	void DFS(int32 here);

private:
	unordered_map<const char*, int32>	_nameToId;
	unordered_map<int32, const char*>	_idToName;
	stack<int32>						_lockStack;
	map<int32, set<int32>>				_lockHistory;

	Mutex _lock;
	vector<int32>	_discoveredOrder; 
	int32			_discoveredCount = 0;
	vector<bool>	_finished; 
	vector<int32>	_parent;
};

