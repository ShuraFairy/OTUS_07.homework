
#pragma once
 
class Observer
{
public:
	virtual~Observer() = default;
	virtual void update(const long long int& time, const std::vector<std::string>& v_stream) = 0;
};