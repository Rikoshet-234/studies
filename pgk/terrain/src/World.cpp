#include "World.hpp"

#include <sstream>
#include <fstream>
#include <stdexcept>
#include <algorithm>


World::World(const std::vector<const char *> &files)
{
	for (auto &filepath : files) {
		loadSector(filepath);

		minX = std::min(minX, sectors.back()->getLatitude());
		maxX = std::max(maxX, sectors.back()->getLatitude() + 1);
		minY = std::min(minY, sectors.back()->getLongtitude());
		maxY = std::max(maxY, sectors.back()->getLongtitude() + 1);
	}
}


void World::loadSector(const std::string &filepath)
{
	std::string filename;
	auto seppos = filepath.find_last_of("/\\");
	if (seppos == std::string::npos) {
		filename = filepath;
	} else {
		filename = filepath.substr(seppos + 1);
	}

	char longc, latc;
	int longtitude, latitude;
	std::istringstream is(filename);
	is >> longc >> longtitude >> latc >> latitude;

	switch (longc) {
	case 'n': case 'N':
		longtitude = 0 + longtitude;
		break;
	case 's': case 'S':
		longtitude = 0 - longtitude;
		break;
	default:
		throw std::runtime_error("incorrect filename latitude format");
	}

	switch (latc) {
	case 'e': case 'E':
		latitude = 0 + latitude;
		break;
	case 'w': case 'W':
		latitude = 0 - latitude;
		break;
	default:
		throw std::runtime_error("incorrect filename longtitude format");
	}

	std::ifstream file(filepath, std::ios::binary);
	if (!file.is_open()) {
		throw std::runtime_error(filepath + " does not exist");
	}

	sectors.push_back(unique_ptr<Sector>(
		new Sector(latitude, longtitude, file)
	));
}


void World::decreaseLOD()
{
	for (auto &sector : sectors) {
		sector->decreaseLOD();
	}
}


void World::increaseLOD()
{
	for (auto &sector : sectors) {
		sector->increaseLOD();
	}
}


static inline vec3 sectorCenter(const Sector &sector)
{
	float phi = glm::radians(sector.getLatitude() + 0.5f);
	float theta = glm::radians(sector.getLongtitude() + 0.5f);
	return vec3(
		glm::cos(theta) * glm::cos(phi),
		glm::cos(theta) * glm::sin(phi),
		glm::sin(theta)
	);
}


void World::decreaseFarthestLOD(vec3 position)
{
	if (sectors.empty()) {
		return;
	}

	std::sort(
		sectors.begin(), sectors.end(),
		[&](const unique_ptr<Sector> &a, const unique_ptr<Sector> &b) {
			float dista = glm::distance(sectorCenter(*a), position);
			float distb = glm::distance(sectorCenter(*b), position);
			return dista > distb;
		}
	);

	for (auto &sector : sectors) {
		if (sector->getLOD() < LOD::LEVELS - 1) {
			sector->decreaseLOD();
			break;
		}
	}
}


void World::increaseClosestLOD(vec3 position)
{
	if (sectors.empty()) {
		return;
	}

	std::sort(
		sectors.begin(), sectors.end(),
		[&](const unique_ptr<Sector> &a, const unique_ptr<Sector> &b) {
			float dista = glm::distance(sectorCenter(*a), position);
			float distb = glm::distance(sectorCenter(*b), position);
			return dista < distb;
		}
	);

	for (auto &sector : sectors) {
		if (sector->getLOD() > 0) {
			sector->increaseLOD();
			break;
		}
	}
}
