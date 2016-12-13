#pragma once
#include <string>
#include <map>

typedef std::map<std::string, unsigned long> IdMapType;

class IdMap
{
public:
	static const IdMapType::value_type& Insert(const std::string& id)
	{
		auto it = m_ids.find(id);
		if (it == m_ids.end())
		{
			return *(m_ids.insert(std::make_pair(id, m_counter++)).first);
		}
		else
		{
			return *it;
		}
	}

	static unsigned long GetLocalId(const std::string& id)
	{
		return m_ids.at(id);
	}

private:
	IdMap() {}

	static IdMapType m_ids;
	static unsigned long m_counter;
};

IdMapType IdMap::m_ids;
unsigned long IdMap::m_counter = 0;

class Id
{
public:
	Id(const std::string& id) : m_valuePair(IdMap::Insert(id))
	{}

	unsigned long GetLocalId()
	{
		return m_valuePair.second;
	}

	const std::string& GetId()
	{
		return m_valuePair.first;
	}

private:
	const IdMapType::value_type& m_valuePair;
};