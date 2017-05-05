#pragma once

#include "interfaces.h"

/*class NodeType : public IType
{
private:
	enum
	{
		DecisionNode,
		ActivityFinalNode,
		InitialNode,
		Action
	} m_type;

public:
	NodeType(const std::string& type)
	{
		if (type == "uml:DecisionNode")
			m_type = DecisionNode;
		else if (type == "uml:ActivityFinalNode")
			m_type = ActivityFinalNode;
		else if (type == "uml:InitialNode")
			m_type = InitialNode;
		else if (type == "uml:Action")
			m_type = Action;
		else
			throw(new std::runtime_error("Bad xmi:type of Node"));
	}

	std::string ToString() const
	{
		if (m_type == DecisionNode)
			return "xmi:type=\"uml:DecisionNode\"\n";
		else if (m_type == ActivityFinalNode)
			return "xmi:type=\"uml:ActivityFinalNode\"\n";
		else if (m_type == InitialNode)
			return "xmi:type=\"uml:InitialNode\"\n";
		else if (m_type == Action)
			return "xmi:type=\"uml:Action\"\n";
		else
			return "";
	}
};*/

class Incoming// : public IElement
{
public:
	Incoming(const std::string& id) : m_ref(id)
	{}

	const std::string ToString() const
	{
		return
			"Incoming:\n"
			"\t" + m_ref.ToString();
	}

	Incoming& operator=(const Incoming&);

private:
	const Id m_ref;
};

class Outgoing// : public IElement
{
public:
	Outgoing(const std::string& id) : m_ref(id)
	{}

	const std::string ToString() const
	{
		return
			"Outgoing:\n"
			"\t" + m_ref.ToString();
	}

	Outgoing& operator=(const Outgoing&);

private:
	const Id m_ref;
};

/*class EffectType : public IType
{
private:
enum
{
OpaqueBehavior
} m_type;

public:
EffectType(const std::string& type)
{
if (type == "uml:OpaqueBehavior")
m_type = OpaqueBehavior;
else
throw(new std::runtime_error("Bad xmi:type of Effect"));
}

std::string ToString() const
{
if (m_type == OpaqueBehavior)
return "xmi:type=\"uml:OpaqueBehavior\"\n";
else
return "";
}
};*/

class Effect : public INumerableElement
{
public:
	Effect(const std::string& id, EffectType type, const std::string& body)
		: INumerableElement(id), m_type(type), m_body(body)
	{}

	const std::string ToString() const
	{
		return
			"Effect:\n"
			/*"\t" + m_type->ToString() +*/
			"\t" + m_id.ToString() +
			"\tBody: " + GetBody();
	}

	const EffectType GetType() const
	{
		return m_type;
	}

	Effect& operator=(const Effect&);

	const std::string GetBody() const { return m_body; }
private:
	const EffectType m_type;
	const std::string m_body;
};

class Node : public INumerableElement, public IVisibleElement
{
public:
	Node(const std::string& id, NodeType type, VisibilityType visibility,
		const std::vector<Incoming>& incoming = std::vector<Incoming>(),
		const std::vector<Outgoing>& outgoing = std::vector<Outgoing>(),
		const std::vector<Effect>& effects = std::vector<Effect>()
	)
		: INumerableElement(id), m_type(type), IVisibleElement(visibility),
		m_effects(effects), m_incoming(incoming), m_outgoing(outgoing)
	{}

	const std::string ToString() const
	{
		return
			"Node:\n"
			"\t" + m_type.ToString() +
			"\t" + m_id.ToString();
			/*"\t" + GetVisibilityString(m_visibility);*/
	}

	const NodeType GetType() const
	{
		return m_type;
	}

	void AddEffect(const Effect& effect)
	{
		m_effects.push_back(effect);
	}

	void AddIncoming(const Incoming& incoming)
	{
		m_incoming.push_back(incoming);
	}

	void AddOutgoing(const Outgoing& outgoing)
	{
		m_outgoing.push_back(outgoing);
	}

	std::vector<Effect> GetEffects() const { return m_effects; }
	void SetEffects(std::vector<Effect> val) { m_effects = val; }

	std::vector<Incoming> GetIncoming() const { return m_incoming; }
	void SetIncoming(std::vector<Incoming> val) { m_incoming = val; }

	std::vector<Outgoing> GetOutgoing() const { return m_outgoing; }
	void SetOutgoing(std::vector<Outgoing> val) { m_outgoing = val; }

private:
	const NodeType m_type;
	std::vector<Effect> m_effects;
	std::vector<Incoming> m_incoming;
	std::vector<Outgoing> m_outgoing;
};