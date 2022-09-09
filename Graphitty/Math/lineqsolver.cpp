#include "lineqsolver.h"

std::vector<double> solveLes(std::vector<std::vector<double>> les)
{
  unsigned int degree = les.size();
  for (unsigned int i = 0; i < degree; i++)
  {
    if (les.at(i).at(i) == 0.0)
    {
      for (unsigned int u = i + 1; u < degree; u++)
      {
        if (les.at(u).at(i) == 0.0)
        {
          continue;
        }

        std::swap(les.at(i), les.at(u));
        break;
      }
    }

    double valueAtBegin = les.at(i).at(i);
    auto line = &les.at(i);
    if (valueAtBegin != 1.0)
    {
      for (unsigned int v = i; v < line->size(); v++)
      {
        line->at(v) /= valueAtBegin;
      }
    }

    for (unsigned int u = 0; u < degree; u++)
    {
      if (u == i)
      {
        continue;
      }

      auto lineOther = &les.at(u);
      double valueAtBeginOther = lineOther->at(i);
      if (valueAtBeginOther == 0.0)
      {
        continue;
      }

      for (unsigned int v = i; v < line->size(); v++)
      {
        lineOther->at(v) -= line->at(v) * valueAtBeginOther;
      }
    }
  }

  std::vector<double> result;
  for (auto line : les)
  {
    result.push_back(line.back());
  }

  return result;
}
