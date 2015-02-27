#ifndef LINEAR_VELOCITY_FIELDS_H
#define LINEAR_VELOCITY_FIELDS_H

#include "eigen3/Eigen/Dense"

typedef float realtype;
#ifdef USE_DOUBLE_PRECISION
typedef double realtype;
#endif
typedef Eigen::Matrix<realtype,Eigen::Dynamic,Eigen::Dynamic> Mat;
typedef Eigen::Matrix<realtype,Eigen::Dynamic,1> Vec;

struct linear_velocity_field{
public:
    linear_velocity_field(Vec target,Mat A,realtype speedcap){
        target_ = target;
        A_ = A;
        speedcap_=speedcap;
    }
    Vec operator()(Vec pos){
        Vec vel = A_*(pos-target_);
        if(vel.norm()>speedcap_){
            vel /= vel.norm()*speedcap_;
        }
        return vel;
    }
private:
    Vec target_;
    Mat A_;
    realtype speedcap_;
};

/**
  //How to use:
  int D = 3;
  Mat A = -0.4*Mat::Identity(D,D)
  Vec target(D);

  linear_velocity_field straightline_field(target,A,0.3);
  // to evaluate the system:
  Vec currpos; // set its value from somewhere
  Vec vel = straightline_field(currpos);

  another example:
    Mat A(D,D); A.setZero();
    A(0,1) = 1;
    A(1,0) = -1;
    A(2,2) = -3;
    linear_velocity_field circular_path(target,A,0.3);

  **/




#endif // LINEAR_VELOCITY_FIELDS_H
