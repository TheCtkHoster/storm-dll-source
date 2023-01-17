#pragma once
 #include <Windows.h>
#include <iostream>
#include <vector>
#include "../eyestep/eyestep.h"
#include "../eyestep/eyestep_utility.h"
#include "../lib_luau/Compiler/include/Luau/Compiler.h"
#include "../lib_luau/VM/include/lua.h"
#include "../lib_luau/VM/include/luaconf.h"
#include "../lib_luau/VM/include/lualib.h"
#include "../lib_luau/VM/src/ltable.h"
uintptr_t base = reinterpret_cast<uintptr_t>(GetModuleHandleA(NULL));
struct r_TValue
{
	union
	{
		std::uintptr_t gc;
		void* p;
		double n;
		int b;
		float v[2];
	} value;

	int extra[1];
	int tt;
};

struct r_CallS
{
	r_TValue* func;
	int nresults;
};
int spawn_address;
int print_address;
int rL;

/* UPDATE THESE ADDIES ONLY */
uintptr_t scheduler_address = base + 0xB16F10;
uintptr_t getstate_address = base + 0x3673B0;
uintptr_t deserialize_detour = base + 0x73C250;
std::uintptr_t jobs_start(0x134);
std::uintptr_t jobs_end(0x138);
std::uintptr_t job_name1(0x10);
std::uintptr_t datamodel_job(0x28);
std::uintptr_t script_context_job(0x130);
using getscheduler_def = uintptr_t(__cdecl*)();
getscheduler_def getscheduler = reinterpret_cast<getscheduler_def>(scheduler_address);
using getstate_def = uintptr_t(__thiscall*)(uintptr_t SC, int* state_type);
getstate_def getstate = reinterpret_cast<getstate_def>(getstate_address);

static uint32_t rbxDaxEncodeOp(uint32_t x, uint32_t mulEven, uint32_t addEven, uint32_t mulOdd, uint32_t addOdd)
{
	uint32_t result = 0;
	uint32_t mask = 1;
	for (size_t i = 0; i < 8 * sizeof(uint32_t); ++i)
	{
		uint32_t bitDesired = mask & x;
		uint32_t bitOdd = mask & (result * mulOdd + addOdd);
		uint32_t bitEven = mask & (result * mulEven + addEven);
		if ((bitEven ^ bitOdd) != bitDesired)
		{
			result |= mask;
		}
		mask <<= 1;
	}
	return result;
}

/* IGNORE JUST SCANNING*/
class scanner
{
private:
	std::uintptr_t taskscheduler = 0xB16F10;
	std::uintptr_t datamodel = 0x28;
	std::uintptr_t script_context = 0x130;
public:
	explicit scanner();
	std::uintptr_t get() const;
	std::uintptr_t get_datamodel() const;
	std::uintptr_t get_script_context() const;
	std::uintptr_t get_global_luastate() const;
	std::vector<std::uintptr_t> get_jobs() const;
	std::uintptr_t get_job_by_name(const std::string& name) const;
	void print_jobs() const;
};

scanner::scanner()
{
	this->taskscheduler = getscheduler();
	std::uintptr_t waiting_scripts_job = this->get_job_by_name("WaitingHybridScriptsJob");
	this->datamodel = *reinterpret_cast<std::uintptr_t*>(waiting_scripts_job + datamodel_job);
	this->script_context = *reinterpret_cast<std::uintptr_t*>(waiting_scripts_job + script_context_job);
}

std::uintptr_t scanner::get() const
{
	return this->taskscheduler;
}

std::uintptr_t scanner::get_datamodel() const
{
	return this->datamodel;
}

std::uintptr_t scanner::get_script_context() const
{
	return this->script_context;
}

std::uintptr_t scanner::get_global_luastate() const
{
	int state_type = 0;
	return getstate(this->get_script_context(), &state_type);
}

void scanner::print_jobs() const
{
	for (std::uintptr_t& job : this->get_jobs())
	{
		std::string* job_name = reinterpret_cast<std::string*>(job + job_name1);
	}
}

std::uintptr_t scanner::get_job_by_name(const std::string& name) const
{
	for (std::uintptr_t& job : this->get_jobs())
	{
		if (std::string* job_name = reinterpret_cast<std::string*>(job + job_name1); name == *job_name)
		{
			return job;
		}
	}
	return NULL;
}

std::vector<std::uintptr_t> scanner::get_jobs() const
{
	std::vector<std::uintptr_t> jobs;
	std::uintptr_t* current_job = *reinterpret_cast<std::uintptr_t**>(this->taskscheduler + jobs_start);
	do {
		jobs.push_back(*current_job);
		current_job += 2;
	} while (current_job != *reinterpret_cast<std::uintptr_t**>(this->taskscheduler + jobs_end));

	return jobs;
}