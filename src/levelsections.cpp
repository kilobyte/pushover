#include "levelsections.h"

const std::string levelSections_c::firstLine = "#!/usr/bin/env pushover";

levelSections_c::levelSections_c(std::istream & stream, bool singleFile) {

  std::vector<std::string> * currentSection = NULL;
  bool atFirstLine = true;
  for (;;) {

    std::streamsize pos = stream.tellg();

    std::string line;
    if (!std::getline(stream, line)) {
      /* end of stream */
      if (!stream.eof())
        throw level_error("unexpected stream error,"
                          " maybe the file does not exist");
      else
        break;
    }

    if (atFirstLine) {
      /* first line */
      if (line != firstLine)
        throw level_error("missing #!-line");
      atFirstLine = false;

    } else if (line == firstLine) {
      /* non-first #!-line */
      if (singleFile)
        throw level_error("unexpected #!-line");
      stream.clear();
      stream.seekg(pos);
      break;

    } else if (line.size() == 0 || line[0] == '+') {
      /* ignored line */

    } else if (line.size() > 0 && line[0] == '|') {
      /* section content */
      if (currentSection == NULL)
        throw level_error("section content before first section name");
      if (line.size() >= 2)
        currentSection->push_back(line.substr(2, std::string::npos));
      else
        currentSection->push_back("");

    } else {
      /* section name */
      sections[line].push_back(std::vector<std::string>());
      currentSection = &sections[line].back();
    }
  }
}

static const std::vector<std::vector<std::string> > empty;

const std::vector<std::vector<std::string> > &
  levelSections_c::getMultiSection(const std::string sectionName) const {

  std::map<std::string, std::vector<std::vector<std::string> > >
    ::const_iterator i = sections.find(sectionName);
  if (i == sections.end())
    return empty;
  else
    return i->second;
}

const std::vector<std::string> &
  levelSections_c::getSingleSection(const std::string sectionName) const {

  const std::vector<std::vector<std::string> > & section =
    getMultiSection(sectionName);

  if (section.size() == 1)
    return section.front();
  else if (section.empty())
    throw level_error("section \"" + sectionName + "\""
                      " is missing");
  else
    throw level_error("section \"" + sectionName + "\""
                      " occurs more than once");
}

const std::string &
  levelSections_c::getSingleLine(const std::string sectionName) const {

  const std::vector<std::string> & singleSection =
    getSingleSection(sectionName);

  if (singleSection.size() == 1)
    return singleSection.front();
  else if (singleSection.empty())
    throw level_error("section \"" + sectionName + "\""
                      " is empty");
  else
    throw level_error("section \"" + sectionName + "\""
                      " has more than one line");
}
