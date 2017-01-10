//-*-C++-*-
/***************************************************************************
 *
 *   Copyright (C) 2016 by Willem van Straten
 *   Licensed under the Academic Free License version 2.1
 *
 ***************************************************************************/

// epsic/src/util/mode.h

#ifndef __mode_H
#define __mode_H

#include "Stokes.h"
#include "Spinor.h"
#include "Matrix.h"
#include "BoxMuller.h"
#include "Minkowski.h"

#include <iostream>

/***************************************************************************
 *
 *  a single source of electromagnetic radiation
 *
 ***************************************************************************/

class mode
{
public:
  mode ();
  virtual ~mode () { }

  virtual void set_Stokes (const Stokes<double>& mean);
  virtual Stokes<double> get_Stokes () { return mean; }

  //! Return the expected mean Stokes parameters
  virtual Stokes<double> get_mean () { return mean; }

  //! Return the expected covariances between the Stokes parameters
  virtual Matrix<4,4, double> get_covariance ()
  { return Minkowski::outer (mean, mean); }

  //! Return the sum of the intensity autocorrelation function
  virtual double get_autocorrelation (unsigned nsample) { return 0; }

  //! Return a random instance of the electric field vector
  virtual Spinor<double> get_field ();

  //! Return BoxMuller object used to generate normally distributed numbers
  virtual BoxMuller* get_normal () { return normal; }
  virtual void set_normal (BoxMuller* n) { normal = n; }

private:
  Stokes<double> mean;
  Jones<double> polarizer;

  BoxMuller* normal;
  double rms;
};
  


class mode_decorator : public mode
{
protected:
  mode* source;

public:
  mode_decorator (mode* s) { source = s; }
  mode* get_source () { return source; }

  void set_Stokes (const Stokes<double>& mean) { source->set_Stokes(mean); }
  Stokes<double> get_Stokes () { return source->get_Stokes(); }

  Matrix<4,4, double> get_covariance () { return source->get_covariance(); }
  Stokes<double> get_mean () { return source->get_mean(); }

  Spinor<double> get_field () { return source->get_field(); }
  BoxMuller* get_normal () { return source->get_normal(); }
  void set_normal (BoxMuller* n) { source->set_normal(n); }
};

#endif
