#ifndef EIGENHASHES_H
#define EIGENHASHES_H

#include <functional>

#include <eigen3/Eigen/Eigen>

namespace std {
    template<>
    struct hash<Eigen::VectorXf> {
       typedef Eigen::VectorXf argument_type;
       typedef size_t result_type;

       result_type operator() (const argument_type& x) const {
          result_type hash = std::hash<result_type>()(x.rows());
          for(int i = 0; i < x.rows(); ++i) {
              hash += std::hash<result_type>()(x[i]);
          }
          return std::hash<result_type>()(hash);
       }
    };
    
    template<>
    struct hash<Eigen::Vector3f> {
       typedef Eigen::Vector3f argument_type;
       typedef size_t result_type;

       result_type operator() (const argument_type& x) const {
          result_type hash = std::hash<result_type>()(x.rows());
          for(int i = 0; i < x.rows(); ++i) {
              hash += std::hash<result_type>()(x[i]);
          }
          return std::hash<result_type>()(hash);
       }
    };
}

#endif /* EIGENHASHES_H */

