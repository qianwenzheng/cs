import scala.collection.mutable.MutableList

object lists {

val listOfPoints : MutableList[Point] = new MutableList[Point]();
val listOfColorPoints : MutableList[ColorPoint] = new MutableList[ColorPoint]();

def addAll0[T](src: MutableList[T], dest: MutableList[T]) : Unit = {
for (o <- src) {
dest += o;
}
}

addAll0(listOfColorPoints, listOfPoints);

}
