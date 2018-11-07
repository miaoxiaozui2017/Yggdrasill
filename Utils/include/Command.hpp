#ifndef COMMAND_HPP
#define COMMAND_HPP
#include <iostream>
#include <list>
// Base Class
class Command
{
public:
	virtual ~Command() = default;
	virtual void Execute() = 0;
protected:
	Command() = default;
};

// Actual Operation
class Obj
{
public:
	Obj() = default;
	void Action() { std::cout << "Obj::Action()" << std::endl; }
};

// Obj Command
class ObjCommand : public Command
{
public:
	ObjCommand(Obj* obj)
		: m_obj(obj)
	{}
	virtual void Execute() { InternalExecute(); }
protected:
	virtual void InternalExecute() { m_obj->Action(); }
private:
	Obj* m_obj;
};

// Simple Command
class SimpleClass
{
public:
	void Action()
	{
		std::cout << "SimpleClass::Action()" << std::endl;
	}
};

template<typename T>
class SimpleCommand : public Command
{
public:
	typedef void (T::* Action)();

	SimpleCommand(T* t, Action a)
		: m_t(t)
		, m_a(a)
	{}

	virtual void Execute()
	{
		(m_t->*m_a)();
	}
private:
	Action m_a;
	T* m_t;
};

// Command management
class MacroCommand : public Command
{
public:
	MacroCommand() = default;
	virtual ~MacroCommand() = default;

	virtual void Add(Command* c)
	{
		m_cmds.insert(m_cmds.end(), c);
	}
	virtual void Remove(Command* c)
	{
		delete c;
	}

	virtual void Execute()
	{
		for (std::list<Command*>::iterator i = m_cmds.begin(); i != m_cmds.end(); i++)
		{
			(*i)->Execute();
		}
		for (std::list<Command*>::iterator i = m_cmds.begin(); i != m_cmds.end(); i++)
		{
			Remove(*i);
		}
		m_cmds.clear();
	}

	// virtual void Reverse();
private:
	std::list<Command*> m_cmds;
};

#endif