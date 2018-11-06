#ifndef CAMSWAP_H
#define CAMSWAP_H
/*
 ************************************
 *** @Xu Xu
 *** @2017.05.10
 *** @NanJing,China
 *** @Hananiahhsu@live.cn
 ************************************
 */
#include <QtGui>
#include <QWidget>
#include "CamDxf/qpointfwithparent.h"
/** we have to start with a random population .
CrossOver with  Sushil J. Louis greedy crossOver
Mutate randomly by swapping 2 (or more cities positions)
Select the elite parents that will generate the new offsprings with Roulette Wheel Selection
**/
//swap the values
void swapVals(QPFWPVector &vect,int pos1,int pos2) ;

/// A chromose actually a possible route
class Chromosome
{
public:

     Chromosome();

     QPFWPVector elements;

     /**
      * create a new chromosome from a given set of points
      * @param points a set of coordinates
      * @return the newly created chromosome
      */
     QPFWPVector createNew(QPFWPVector points);
     int MySize;
     virtual ~Chromosome();
     double routeLength;
     ///@todo for now fitness=lenght change when switching to roulette selection...
     double fitness;

     /**
      * set the chromosome elements (a.k.a the points definig the route)
      * @param points the route elements
      * @param random if set will take route points randomly, elsewhere the points are copied as is
      */
     void setElements(QPFWPVector points,bool random);

     /// create a chromosome of size by copying poiunts as its elments
     void setElements(QPFWPVector points,int size=0);
     /**
      * set the chromosome fitness according to its route length
      * @param fit
      */
     void setFitness(double fit){this->fitness=fit;}

     /**
      *    Calculate the total route length
      */
     void calcRouteLength();
     /**
      *
      * @return Chromosome route length
      */
     double getRouteLength(){return routeLength;}
};


 /// @todo: create a function that return the pop/chromosome size for brievty
/// A population is a set of chromosomes
class Popu
{
public:

     /**
      *        Create a new population
      */
     Popu();
     void computeTotalRoute();
     /**
      * Sort the population according to its content fitness: The chromosome with the best fitness are placed on the top of the queue
      *@todo: See if calling standard arrays function provideid by Qt is possible here
      */
     void sortContent();

     /**
      *        Creates a new Generation from the current Population (a.k.a @see content)
      */
     void createNewGen();
     /**
      *        Start GA optimization
      * @param points unOptimizaed route
      * @param randomtartupPop if set the created population is a random one
      * @return Optimized route
          */
         QPFWPVector init(QPFWPVector points,bool randomtartupPop=true);
     void selectParents(Chromosome* parent1,Chromosome* parent2,int posd);
     Chromosome cross(Chromosome parent1, Chromosome parent2);
         Chromosome crossOver(Chromosome parent1, Chromosome parent2);

     void selectTwoParents(Chromosome* p1,Chromosome* p2,int posd);
     void mutateImprove(int pos);

     void mutateImproveOffspring(Chromosome &toImprove,int pos);

     /**
      *        Mutate the given Chromosome
      * @param pos chromosome Position in the population
      */
     void mutateImproveMe(int pos);
     /**
      * Given an offspring we mutate it at a certain rate and create the new population
      * @param i the Chromosome position to mutate (in the genration)
      */
     void mutate(int i);

     QVector <Chromosome > newGen;

     ///
     double totalRoute;
 private:
     ///The population size : A common value is between 10 and 20
     int popSize;
     int Max_iter;
     int chromoSize;
     /// the Chromosome list a.k.a the population
     QVector <Chromosome > content;

};


class Population
{
public:

     Population(QPFWPVector points);
     virtual ~Population();
     QVector <Chromosome *> content;
     double totalRoute;
     int popSize;
     void selectElite();
     void mutate(Chromosome *offspring);
      Chromosome *crossoverSimple(Chromosome *parent1, Chromosome *parent2);
     Chromosome *crossover(Chromosome *parent1, Chromosome *parent2);
};

 class GAThread : public QThread
 {
     Q_OBJECT

 protected:
     void run();
 };


#endif // CAMSWAP_H
