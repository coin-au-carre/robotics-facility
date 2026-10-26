package Micro

import Micro.Actions.Basic.Gather
import ProxyBwapi.UnitInfo.FriendlyUnitInfo
import scala.runtime.{LazyBoolean, LazyRef}

object GatherEmptyThreatsTest extends App {
  val unsafeField = classOf[sun.misc.Unsafe].getDeclaredField("theUnsafe")
  unsafeField.setAccessible(true)
  val unsafe = unsafeField.get(null).asInstanceOf[sun.misc.Unsafe]
  val unit = unsafe.allocateInstance(classOf[FriendlyUnitInfo]).asInstanceOf[FriendlyUnitInfo]
  def set(name: String, value: Int): Unit = {
    val field = Iterator.iterate[Class[_]](unit.getClass)(_.getSuperclass)
      .takeWhile(_ != null).flatMap(_.getDeclaredFields).find(f => f.getName == name || f.getName.endsWith("$$" + name)).get
    field.setAccessible(true)
    field.setInt(unit, value)
  }

  // Invoke the actual Scala 2.12 local lazy predicate without starting BWAPI.
  val predicate = Gather.getClass.getDeclaredMethods
    .find(_.getName.startsWith("stabbingLethal$lzycompute$")).get
  predicate.setAccessible(true)
  var passed = 0
  for (health <- Seq(0, -1, 1, 40); view <- Seq(false, true)) {
    set("_hitPoints", health)
    val threats = new LazyRef[Seq[ProxyBwapi.UnitInfo.UnitInfo]]
    val empty = Vector.empty[ProxyBwapi.UnitInfo.UnitInfo]
    threats.initialize(if (view) empty.view else empty)
    val lethal = predicate.invoke(null, new LazyBoolean, unit, threats).asInstanceOf[Boolean]
    assert(!lethal, s"empty threats must not be lethal: health=$health view=$view")
    passed += 1
    println(s"PASS empty threats: health=$health view=$view")
  }
  println(s"$passed Gather empty-threat regressions passed")
}
