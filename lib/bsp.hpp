#ifndef BSP_HPP
#define BSP_HPP

class Filter {
  enum Dimension {
    DIMENSION_X,
    DIMENSION_Y,
    DIMENSION_Z,
  } m_dimension;
  float m_value;

  float Get(glm::vec3 vec, Dimension dim) {
    switch (dim) {
      case DIMENSION_X:
        return vec.x;
      case DIMENSION_Y:
        return vec.y;
      case DIMENSION_Z:
        return vec.z;
    }
  }

  int Compare(glm::vec3 begin, glm::vec3 end) {
    int value1 = Get(begin, m_dimension);
    int value2 = Get(end, m_dimension);

    if (value1 < m_value && value2 < m_value)
      return -1;
    else if (value1 > m_value && value2 > m_value)
      return 1;
    else
      return 0;
  }
};

struct BSPNode {};

template <class T>
class BSP {
 private:
};

#endif
