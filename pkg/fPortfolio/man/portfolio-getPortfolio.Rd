\name{portfolio-getPortfolio}



\alias{getData.fPORTFOLIO}
\alias{getSeries.fPORTFOLIO}
\alias{getNAssets.fPORTFOLIO}
\alias{getUnits.fPORTFOLIO}
\alias{getStatistics.fPORTFOLIO}
\alias{getMean.fPORTFOLIO}
\alias{getCov.fPORTFOLIO}
\alias{getMu.fPORTFOLIO}
\alias{getSigma.fPORTFOLIO}
\alias{getEstimator.fPORTFOLIO}

\alias{getSpec.fPORTFOLIO}
\alias{getModel.fPORTFOLIO}
\alias{getType.fPORTFOLIO}
\alias{getOptimize.fPORTFOLIO}
\alias{getEstimator.fPORTFOLIO}
\alias{getTailRisk.fPORTFOLIO}
\alias{getParams.fPORTFOLIO}
\alias{getOptim.fPORTFOLIO}
\alias{getSolver.fPORTFOLIO}
\alias{getTrace.fPORTFOLIO}

\alias{getConstraints.fPORTFOLIO}
\alias{getConstraintsTypes}

\alias{getPortfolio.fPORTFOLIO}
\alias{getWeights.fPORTFOLIO}
\alias{getTargetReturn.fPORTFOLIO}
\alias{getTargetRisk.fPORTFOLIO}
\alias{getAlpha.fPORTFOLIO}
\alias{getRiskFreeRate.fPORTFOLIO}
\alias{getNFrontierPoints.fPORTFOLIO}
\alias{getStatus.fPORTFOLIO}

\alias{getCovRiskBudgets.fPORTFOLIO}
\alias{getTailRiskBudgets.fPORTFOLIO}

\alias{getA.fPORTFOLIO}
\alias{getControl.fPORTFOLIO}
\alias{getObjective.fPORTFOLIO}
\alias{getOptions.fPORTFOLIO}


\title{Portfolio Class Extractors}


\description{
  
    A collection and description of functions 
    allowing to get information about an object
    of class fPORTFOLIO. 
    \cr
    
    The functions are:
    
    \tabular{ll}{
    \code{getData} \tab Extracts ..., \cr
    \code{getSeries} \tab Extracts ..., \cr
    \code{getStatistics} \tab Extracts ..., \cr
    \code{getNAssets} \tab Extracts ..., \cr
    \code{getSpec} \tab Extracts ..., \cr
    \code{getType} \tab Extracts ..., \cr
    \code{getEstimator} \tab Extracts ..., \cr
    \code{getParams} \tab Extracts ..., \cr
    \code{getSolver} \tab Extracts ..., \cr
    \code{getTrace} \tab Extracts ..., \cr
    \code{getConstraints} \tab Extracts ..., \cr
    \code{getPortfolio} \tab Extracts ..., \cr
    \code{getWeights} \tab Extracts ..., \cr
    \code{getTargetReturn} \tab Extracts ..., \cr
    \code{getTargetRisk} \tab Extracts ..., \cr
    \code{getAlpha} \tab Extracts ..., \cr
    \code{getRiskFreeRate} \tab Extracts ..., \cr
    \code{getNFrontierPoints} \tab Extracts ..., \cr
    \code{getStatus} \tab Extracts ..., \cr
    \code{getCovRiskBudgets} \tab Extracts ..., \cr
    \code{getTailRiskBudgets} \tab Extracts ... . }
    
}
    

\usage{
\method{getData}{fPORTFOLIO}(object)
\method{getSeries}{fPORTFOLIO}(object)
\method{getNAssets}{fPORTFOLIO}(object)
\method{getUnits}{fPORTFOLIO}(x)
\method{getStatistics}{fPORTFOLIO}(object)
\method{getMean}{fPORTFOLIO}(object)
\method{getCov}{fPORTFOLIO}(object)
\method{getMu}{fPORTFOLIO}(object)
\method{getSigma}{fPORTFOLIO}(object)
\method{getEstimator}{fPORTFOLIO}(object)

\method{getSpec}{fPORTFOLIO}(object)
\method{getModel}{fPORTFOLIO}(object)
\method{getType}{fPORTFOLIO}(object)
\method{getOptimize}{fPORTFOLIO}(object)
\method{getEstimator}{fPORTFOLIO}(object)
\method{getTailRisk}{fPORTFOLIO}(object)
\method{getParams}{fPORTFOLIO}(object)
\method{getOptim}{fPORTFOLIO}(object)
\method{getSolver}{fPORTFOLIO}(object)
\method{getTrace}{fPORTFOLIO}(object)

\method{getConstraints}{fPORTFOLIO}(object)

\method{getPortfolio}{fPORTFOLIO}(object)
\method{getWeights}{fPORTFOLIO}(object)
\method{getTargetReturn}{fPORTFOLIO}(object)
\method{getTargetRisk}{fPORTFOLIO}(object)
\method{getAlpha}{fPORTFOLIO}(object)
\method{getRiskFreeRate}{fPORTFOLIO}(object)
\method{getNFrontierPoints}{fPORTFOLIO}(object)
\method{getStatus}{fPORTFOLIO}(object)

\method{getCovRiskBudgets}{fPORTFOLIO}(object)
\method{getTailRiskBudgets}{fPORTFOLIO}(object)


\method{getA}{fPORTFOLIO}(object)
\method{getControl}{fPORTFOLIO}(object)
\method{getObjective}{fPORTFOLIO}(object)
\method{getOptions}{fPORTFOLIO}(object)

}



\arguments{

    \item{object}{
        an object of class \code{fPORTFOLIO}, containing slots call, data,
        specification, constraints, portfolio, title, description.
        }
    \item{x}{
        an object of class \code{fPORTFOLIO}, containing slots call, data,
        specification, constraints, portfolio, title, description.
        }
}


\references{

Wuertz, D., Chalabi, Y., Chen W., Ellis A. (2009);
    \emph{Portfolio Optimization with R/Rmetrics}, 
    Rmetrics eBook, Rmetrics Association and Finance Online, Zurich.
    
}


\keyword{models}

