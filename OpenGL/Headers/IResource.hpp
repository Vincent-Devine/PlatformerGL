#pragma once

#include <string>

namespace Resources
{
	class IResource
	{
		// Attribute
	protected:
		std::string path1;
		std::string path2;
		std::string name;
		int id;

		// Methode
	public:
		virtual ~IResource() {};

		virtual void Init() {};

		virtual void SetPath1(const std::string p_path1) { path1 = p_path1; };
		virtual void SetPath2(const std::string p_path2) { path1 = p_path2; };
		virtual void SetName(const std::string p_name) { name = p_name; };

		virtual std::string GetPath1() const { return path1; };
		virtual std::string GetPath2() const { return path2; };
		virtual std::string GetName() const { return name; };
	};
}