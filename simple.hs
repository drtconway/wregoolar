
data RE = Epsilon
        | Atom Char
        | Star RE
        | Conj RE RE 
        | Disj RE RE 

interleave :: [t] -> [t] -> [t]
interleave [] ys        = ys
interleave (x:xs) ys    = x : (ys `interleave` xs)

concatMerge :: [[t]] -> [[t]] -> [[t]]
concatMerge [] ys = []
concatMerge xs [] = []
concatMerge (x:xs) (y:ys) = (x ++ y) : ([x ++ w | w <- ys] `interleave` [v ++ y | v <- xs] `interleave` (concatMerge xs ys))

enumerate :: RE -> [String]
enumerate Epsilon           = [""]
enumerate (Atom c)          = [[c]]
enumerate (Disj lhs rhs)    = (enumerate lhs) `interleave` (enumerate rhs)
enumerate (Conj lhs rhs)    = (enumerate lhs) `concatMerge` (enumerate rhs)
enumerate (Star r)          = "" : ((enumerate r) `concatMerge` (enumerate (Star r)))
