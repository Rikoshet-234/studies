
signature ORDERED = 
sig
  type T

  val lt : T * T -> bool
  val gt : T * T -> bool
  val eq : T * T -> bool
end

signature HEAP_WITH_INFO = 
sig
  structure Priority : ORDERED
  type 'a Heap

  val empty : 'a Heap
  val isEmpty : 'a Heap -> bool
  val insert : Priority.T * 'a * 'a Heap -> 'a Heap
  val pop : 'a Heap -> 'a Heap
  val merge : 'a Heap * 'a Heap -> 'a Heap
  val minimum : 'a Heap -> Priority.T * 'a
end

functor LazyBinomialHeap (P : ORDERED) (* TODO: : HEAP_WITH_INFO *) = 
struct
  open SMLofNJ.Susp

  structure Priority = P
  datatype 'a Tree = Node of int * Priority.T * 'a * 'a Tree list
  type 'a Heap = 'a Tree list susp

  (* TODO: val empty = delay ((fn _ => []) : unit -> 'a Tree list) *)
  fun isEmpty h = (null o force) h

  fun rank (Node (r, _, _, _)) = r
  fun link (a as Node (ar, ap, av, ats), b as Node (br, bp, bv, bts)) =
    if Priority.lt (ap, bp)
    then Node (ar + 1, ap, av, b :: ats)
    else Node (br + 1, bp, bv, a :: bts)

  fun put (t', [])             = [t']
    | put (t', tts as t :: ts) =
      if rank t' < rank t
      then t' :: tts
      else put (link (t, t'),  ts)

  fun merge' (ts, [])                         = ts
    | merge' ([], ts)                         = ts
    | merge' (ha as a :: ats, hb as b :: bts) =
      if rank a < rank b
      then a :: merge' (ats, hb)
      else if rank b < rank a
           then b :: merge' (ha, bts)
           else put (link (a, b), merge' (ats, bts))

  fun minimum' (Node (_, p, v, _) :: []) = (p, v)
    | minimum' (Node (_, p, v, _) :: ts) =
      let val (mp, mv) = minimum' ts
      in
        if Priority.lt (p, mp)
        then (p, v)
        else (mp, mv)
      end

  fun pop' (Node (_, _, _, ts)) = delay (fn _ => rev ts)

  fun insert (p, v, h) = delay (fn _ => merge' ([Node (0, p, v, [])], force h))
  fun merge (a, b) = delay (fn _ => merge' (force a, force b))
  fun minimum h = minimum' (force h)
  fun pop h = pop' (force h)
end

functor BootstrapHeap (H : HEAP_WITH_INFO) : HEAP_WITH_INFO = 
struct
  structure Priority = H.Priority
  datatype 'a Heap = Empty | Node of Priority.T * 'a * 'a Heap H.Heap

  val empty = Empty
  fun isEmpty Empty = true
    | isEmpty _     = false

  fun minimum (Node (v, p, _)) = (v, p)

  fun merge (a, Empty) = a
    | merge (Empty, b) = b
    | merge (a as Node (ap, av, ah), b as Node (bp, bv, bh)) =
      if Priority.lt (ap, bp)
      then Node (ap, av, H.insert (bp, b, ah))
      else Node (bp, bv, H.insert (ap, a, bh))

  fun insert (p, v, h) = merge (Node (p, v, H.empty), h)

  fun pop (Node (_, _, h)) =
    if H.isEmpty h
    then Empty
    else let val (_, Node (p, v, h')) = H.minimum h
             val h'' = H.pop h
         in Node (p, v, H.merge (h', h'')) end
end
