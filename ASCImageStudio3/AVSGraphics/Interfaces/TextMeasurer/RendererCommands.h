#pragma once

#include <utility>
#include <stack>
#include "SharedPtr.h"

using std::pair;
using std::stack;

#ifdef TME_RENDERER_COMMAND_DEBUG
#define SEND_TO_STACK(command) RendererCommandStack::sendCommand(command)
#else
#define SEND_TO_STACK(command)
#endif

#define USE_NEW_COMMAND
#ifdef USE_NEW_COMMAND
#define __UUIDOF__IAVSRENDERER2  __uuidof(IAVSRenderer2)
#else
#define __UUIDOF__IAVSRENDERER2  __uuidof(IAVSFontManager)
#endif

class RendererCommand {
public:
	enum RendererCommandType {
		BEGIN, END
	};
	enum RendererCommandName {
		COMMON, HEADER, TABLE, BLOCK
	};

private:
	RendererCommandType _type;
	RendererCommandName _name;

	bool _added;

public:
	RendererCommand(const RendererCommandName& name = COMMON
			, const RendererCommandType& type = BEGIN) 
			: _name(name), _type(type)
			, _added(false) {
	}

public:
	virtual void send(IAVSRenderer2* renderer) = 0;

public:
	void setType(const RendererCommandType& type) {
		_type = type;
	}
	RendererCommandType getType() const {
		return _type;
	}
	RendererCommandName getName() const {
		return _name;
	}
	void setAdded(const bool& added) {
		_added = added;
	}
	bool isAdded() const {
		return _added;
	}

	virtual bool isNeeded() const {
		return true;
	}
	
	bool isEqualNameWith(const RendererCommand* command) {
		return _name == command->_name;
	}

	virtual void setRect(const RectF& rect) {}
};

class RendererCommandStack {
private:
	typedef pair<RendererCommand::RendererCommandName
			, RendererCommand::RendererCommandType> StubCommand;
	stack<StubCommand> _commands;

public:
	void processCommand(const RendererCommand::RendererCommandName& name
			, const RendererCommand::RendererCommandType& type) {
		if (name == RendererCommand::COMMON)
			return;
		if (type == RendererCommand::BEGIN)
			_commands.push(StubCommand(name, type));
		else {
			assert(!_commands.empty());
			StubCommand command = _commands.top();
			assert(command.first == name);
			assert(command.second == RendererCommand::BEGIN);
			_commands.pop();
		}
	}

public:
	static void sendCommand(const CArray<SharedPtr<RendererCommand>>& commands) {
		for (int i = 0; i < (int) commands.GetCount(); ++i)
			sendCommand(commands[i]);
	}
	static void sendCommand(const SharedPtr<RendererCommand>& command) {
		static RendererCommandStack stack;
		stack.processCommand(command->getName(), command->getType());
	}
	static void sendCommand(const RendererCommand* command) {
		static RendererCommandStack stack;
		stack.processCommand(command->getName(), command->getType());
	}
};

class RendererCommandSender {
private:
	CArray<SharedPtr<RendererCommand>> _commands;

protected:
	~RendererCommandSender() {
		_commands.RemoveAll();
	}

public:
	void setRendererCommands(const CArray<SharedPtr<RendererCommand>>& commands) {
		_commands.Append(commands);
		SEND_TO_STACK(commands);
	}
	void addRendererCommand(const SharedPtr<RendererCommand>& command) {
		_commands.Add(command);
		SEND_TO_STACK(command);
	}
protected:
	void sendRendererCommands(IAVSRenderer* renderer
			, const RendererCommand::RendererCommandType& type) {
		if (isRendererCommandExist()) {
			IAVSRenderer2* renderer2 = getRenderer2(renderer);

			if (renderer2 != NULL)
				for (int i = 0; i < (int) _commands.GetCount(); ++i)
					if (_commands[i]->getType() == type)
						_commands[i]->send(renderer2);

			RELEASEINTERFACE(renderer2);
		}
	}

private:
	bool isRendererCommandExist() const {
		return _commands.GetCount() != 0;
	}
	IAVSRenderer2* getRenderer2(IAVSRenderer* renderer) const {
		IAVSRenderer2* renderer2 = NULL;
		if (!SUCCEEDED(renderer->QueryInterface(__UUIDOF__IAVSRENDERER2, (void**)&renderer2)))
			renderer2 = NULL;

		return renderer2;
	}
};

typedef SharedPtr<RendererCommand> RCommand;
	
class PageBreakRendererCommand : public RendererCommand {
public:
	virtual void send(IAVSRenderer2* renderer) {
		renderer->CommandLong(c_nPageBreak);
	}
};
class TableStartRendererCommand : public RendererCommand {
private:
	CString _xml;
	float _top;

public:
	TableStartRendererCommand(const CString& xml)
			: RendererCommand(TABLE, BEGIN)
			, _xml(xml) {
	}

public:
	virtual void send(IAVSRenderer2* renderer) {
		IAVSRenderer* pRend = NULL;
		renderer->QueryInterface(__uuidof(IAVSRenderer), (void**)&pRend);
		pRend->BeginCommand(c_nTable);
		RELEASEINTERFACE(pRend);

		BSTR bstrXml = _xml.AllocSysString();
		renderer->CommandString2(c_nTableXml, bstrXml);
		renderer->CommandDouble2(c_nTableTop, _top);
		SysFreeString(bstrXml);
	}

public:
	virtual void setRect(const RectF& rect) {
		_top = rect.Y * 25.4f / 72;
	}
};

class TableEndRendererCommand : public RendererCommand {
public:
	TableEndRendererCommand() 
			: RendererCommand(TABLE, END) {
	}

public:
	virtual void send(IAVSRenderer2* renderer) {
		IAVSRenderer* pRend = NULL;
		renderer->QueryInterface(__uuidof(IAVSRenderer), (void**)&pRend);
		pRend->EndCommand(c_nTable);
		RELEASEINTERFACE(pRend);
	}
};

class HeaderStartRendererCommand : public RendererCommand {
public:
	HeaderStartRendererCommand() 
			: RendererCommand(HEADER, BEGIN) {
	}

public:
	virtual void send(IAVSRenderer2* renderer) {
		IAVSRenderer* pRend = NULL;
		renderer->QueryInterface(__uuidof(IAVSRenderer), (void**)&pRend);
		pRend->BeginCommand(c_nHeader);
		RELEASEINTERFACE(pRend);
		SEND_TO_STACK(this);
	}
};

class HeaderEndRendererCommand : public RendererCommand {
public:
	HeaderEndRendererCommand() 
			: RendererCommand(HEADER, END) {
	}

public:
	virtual void send(IAVSRenderer2* renderer) {
		IAVSRenderer* pRend = NULL;
		renderer->QueryInterface(__uuidof(IAVSRenderer), (void**)&pRend);
		pRend->EndCommand(c_nHeader);
		RELEASEINTERFACE(pRend);
		SEND_TO_STACK(this);
	}
};
class BlockStartRendererCommand : public RendererCommand {
private:
	int _headingLevel;
	int _numberingLevel;
	int _align;

public:
	BlockStartRendererCommand(const int& headingLevel, const int& numberingLevel
			, const int& align) 
			: RendererCommand(BLOCK, BEGIN)
			, _headingLevel(headingLevel), _numberingLevel(numberingLevel)
			, _align(align) {
	}

public:
	virtual void send(IAVSRenderer2* renderer2) {
		if (isNeeded()) {
			IAVSRenderer* renderer = NULL;
			renderer2->QueryInterface(__uuidof(IAVSRenderer), (void**)&renderer);
			renderer->BeginCommand(c_nBlock);

			if (_headingLevel > 0)
				renderer2->CommandLong2(c_nBlockHeadingLevel, _headingLevel);
			if (_numberingLevel > 0)
				renderer2->CommandLong2(c_nBlockNumberingLevel, _numberingLevel);
			renderer2->CommandLong2(c_nBlockHorzAlign, _align);
			SEND_TO_STACK(this);
		}
	}

public:
	bool isNeeded() const {
		return true;
	}

	friend class BlockEndRendererCommand;
};

class BlockEndRendererCommand : public RendererCommand {
private:
	RCommand _beginBlockCommand;

public:
	BlockEndRendererCommand(const RCommand& beginBlockCommand) 
			: RendererCommand(BLOCK, END)
			, _beginBlockCommand(beginBlockCommand) {
	}

public:
	virtual void send(IAVSRenderer2* renderer2) {
		if (_beginBlockCommand->isNeeded()) {
			IAVSRenderer* renderer = NULL;
			renderer2->QueryInterface(__uuidof(IAVSRenderer), (void**)&renderer);
			renderer->EndCommand(c_nBlock);
			SEND_TO_STACK(this);
		}
	}
};

class ColumnCountRendererCommand : public RendererCommand {
private:
	int _columnCount;

public:
	ColumnCountRendererCommand(const int& columnCount) 
		: _columnCount(columnCount) {
	}

public:
	virtual void send(IAVSRenderer2* renderer2) {
		renderer2->CommandLong2(c_nColumnCount, _columnCount);
	}
};