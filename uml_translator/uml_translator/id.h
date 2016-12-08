#pragma once
#include <string>
#include <map>

class IdMap
{
public:
	static void Insert(const std::string& id, unsigned long localId)
	{
		m_ids.insert(std::make_pair(id, localId));
	}

	static unsigned long GetLocalId(const std::string& id)
	{
		return m_ids.at(id);
	}

private:
	static std::map<std::string, unsigned long> m_ids;
};

std::map<std::string, unsigned long> IdMap::m_ids;

class Id
{
public:
	Id(const std::string& id) : m_id(id)
	{
		static unsigned long counter = 0;
		m_localId = counter++;
		IdMap::Insert(id, counter++);
	}

	unsigned long GetLocalId()
	{
		return m_localId;
	}

	const std::string& GetId()
	{
		return m_id;
	}

private:
	const std::string m_id;
	unsigned long m_localId;
};