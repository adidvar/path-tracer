
class BVH {
 public:
  BVH(std::vector<Object*> objects);
  
  bool Intersect(Ray ray, Hit &out_hit) const;
  

};

