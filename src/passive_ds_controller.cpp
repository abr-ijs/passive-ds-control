#include "passive_ds_controller.h"

#include <iostream>


void orthonormalize(Mat & basis){
    assert(basis.rows() == basis.cols());
    uint dim = basis.rows();
    basis.col(0).normalize();
    for(uint i=1;i<dim;i++){
        for(uint j=0;j<i;j++)
            basis.col(i) -= basis.col(j).dot(basis.col(i))*basis.col(j);
        basis.col(i).normalize();
    }
}


void assert_orthonormal(Mat & basis){
    uint dim = basis.cols();
    for (int i = 0; i < dim; ++i) {
        assert(fabs(basis.col(i).norm()-1.0) < FLOATEQUAL);
        for (int j = 0; j < i; ++j) {
            assert(fabs(basis.col(i).dot(basis.col(j)))<FLOATEQUAL);
        }

    }
}

PassiveDSController::PassiveDSController(int dim,realtype damping_eigval0,realtype damping_eigval1)
{
    damping_.resize(dim,dim);
    damping_eigval_.resize(dim,dim);
    set_damping_eigval(damping_eigval0,damping_eigval1);
    basis_.resize(dim,dim);
    basis_.setRandom();
    orthonormalize(basis_);
    assert_orthonormal(basis_);
}


void PassiveDSController::ComputeOrthonormalBasis(Vec dir){
    assert(dir.rows()==basis_.rows());
    basis_.col(0)=dir;
    orthonormalize(basis_);
}

void PassiveDSController::set_damping_eigval(realtype damping_eigval0,realtype damping_eigval1)
{
    damping_eigval_.setZero();
    damping_eigval_(0,0)=damping_eigval0;
    for(int i=1;i<damping_eigval_.rows();i++)
        damping_eigval_(i,i)=damping_eigval1;
}

Mat PassiveDSController::ComputeDamping(Vec vel)
{
    // only proceed of we have a minimum velocity norm!
    if(vel.norm() > MINSPEED)
        ComputeOrthonormalBasis(vel);
    // otherwise just use the last computed basis
    damping_ = basis_*damping_eigval_*basis_.transpose();
    return damping_;
}