/*
 * Copyright 2010-2016 OpenXcom Developers.
 *
 * This file is part of OpenXcom.
 *
 * OpenXcom is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * OpenXcom is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with OpenXcom.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "RuleInterface.h"
#include "Mod.h"
#include <climits>

namespace OpenXcom
{

/**
 * Creates a blank ruleset for a certain
 * type of interface, containing an index of elements that make it up.
 * @param type String defining the type.
 */
RuleInterface::RuleInterface(const std::string & type) : _type(type), _sound(-1)
{
}

RuleInterface::~RuleInterface()
{
}

/**
 * Loads the elements from a YAML file.
 * @param node YAML node.
 */
void RuleInterface::load(const YAML::YamlNodeReader& reader, Mod *mod)
{
	if (const auto& parent = reader["refNode"])
	{
		load(parent, mod);
	}

	reader.tryRead("palette", _palette);
	reader.tryRead("parent", _parent);
	reader.tryRead("backgroundImage", _backgroundImage);
	reader.tryRead("altBackgroundImage", _altBackgroundImage);
	reader.tryRead("music", _music);
	mod->loadSoundOffset(_type, _sound, reader["sound"], "GEO.CAT");
	for (const auto& elementReader : reader["elements"].children())
	{
		Element element;
		if (elementReader["size"])
		{
			std::pair<int, int> pos = elementReader["size"].readVal<std::pair<int, int> >();
			element.w = pos.first;
			element.h = pos.second;
		}
		else
		{
			element.w = element.h = INT_MAX;
		}
		if (elementReader["pos"])
		{
			std::pair<int, int> pos = elementReader["pos"].readVal<std::pair<int, int> >();
			element.x = pos.first;
			element.y = pos.second;
		}
		else
		{
			element.x = element.y = INT_MAX;
		}
		element.color = elementReader["color"].readVal<int>(INT_MAX);
		element.color2 = elementReader["color2"].readVal<int>(INT_MAX);
		element.border = elementReader["border"].readVal<int>(INT_MAX);
		element.custom = elementReader["custom"].readVal<int>(0);
		element.TFTDMode = elementReader["TFTDMode"].readVal(false);

		std::string id = elementReader["id"].readVal<std::string>("");
		_elements[id] = element;
	}
}

/**
 * Retrieves info on an element
 * @param id String defining the element.
 */
Element *RuleInterface::getElement(const std::string &id)
{
	auto i = _elements.find(id);
	if (_elements.end() != i) return &i->second; else return 0;
}

const std::string &RuleInterface::getPalette() const
{
	return _palette;
}

const std::string &RuleInterface::getParent() const
{
	return _parent;
}

const std::string &RuleInterface::getBackgroundImage() const
{
	return _backgroundImage;
}

const std::string &RuleInterface::getAltBackgroundImage() const
{
	return _altBackgroundImage;
}

const std::string &RuleInterface::getMusic() const
{
	return _music;
}

int RuleInterface::getSound() const
{
	return _sound;
}

}
