using OSQP
using SparseArrays
using LinearAlgebra

#Convert model from continuous to discrete time
function c2d(A, B, Δt)
    (nx, nu) = size(B)
    M = exp([Array(A) * Δt  Array(B) * Δt;
    zeros(nu, nx + nu)])
    Ad = M[1:nx, 1:nx]
    Bd = M[1:nx, nx + 1:nx + nu]
   	return  sparse(Ad), sparse(Bd)
end

const M = 10;
const θ = π / 4;
const d = 0.5;
A = sparse([
    [0 0 0 1. 0 0];
    [0 0 0 0 1. 0];
    [0 0 0 0 0 1.];
    [0 0 0 0 0 0];
    [0 0 0 0 0 0];
    [0 0 0 0 0 0]
])
B = sparse([
    [0 0 0 0];
    [0 0 0 0];
    [0 0 0 0];
    [(sqrt(2) / M) (-sqrt(2) / M) (-sqrt(2) / M) (sqrt(2) / M)];
    [(sqrt(2) / M)  (sqrt(2) / M)  (sqrt(2) / M) (sqrt(2) / M)];
    [(cos(π / 4 - θ) * d)  (cos(π / 4 - θ) * d) (cos(π / 4 - θ) * d) (cos(π / 4 - θ) * d)]
])
Ad, Bd = c2d(A, B, 0.05);
(nx, nu) = size(Bd)

# Constraints
u0 = 10.5916
umin = [9.6, 9.6, 9.6, 9.6] .- u0
umax = [13., 13., 13., 13.] .- u0
xmin = [-π / 6, -pi / 6, -Inf,  - Inf,  - Inf,  -1]
xmax = [π / 6,  π / 6,	Inf,  Inf,  Inf,  Inf]

# Objective function
Q = spdiagm(0 => [0., 0., 10., 10., 10., 10.])
QN = Q
R =  sparse(0.1I, 4, 4)

# Initial and reference states
x0 = spzeros(6, 1)
xr = [0.,0.,1.,0.,0.,0.]

# Prediction horizon
N = 10
# Cast MPC problem to a QP: x = (x(0),x(1),...,x(N),u(0),...,u(N-1))
# - quadratic objective
P = blockdiag(kron(sparse(1.0I, N, N), Q), QN,
                       kron(sparse(1.0I, N, N), R))
# - linear objective
q = vcat(kron(ones(N, 1), (-Q * xr)), (-QN * xr), spzeros(N * nu, 1))
# - linear dynamics
Ax = kron(sparse(1.0I, N + 1, N + 1), -sparse(1.0I, nx, nx)) + kron(spdiagm(-1 => ones(N + 1 - 1)), Ad)
Bu = kron(vcat(spzeros(1, N), sparse(1.0I, N, N)), Bd)
Aeq = hcat(Ax, Bu)
leq = hcat(-x0', spzeros(1, N * nx))
ueq = leq
# - input and state constraints
Aineq = spdiagm(0 => ones((N + 1) * nx + N * nu))
lineq = hcat(kron(ones(1, N + 1), xmin'), kron(ones(1, N), umin'))
uineq = hcat(kron(ones(1, N + 1), xmax'), kron(ones(1, N), umax'))
# - OSQP constraints
A = vcat(Aeq, Aineq)
l = hcat(leq, lineq)
u = hcat(ueq, uineq)

# Create an OSQP object

prob = OSQP.Model()
# Setup workspace
q = reshape(Array(q), length(q))
l = reshape(Array(l), length(l))
u = reshape(Array(u), length(u))
OSQP.setup!(prob;P = P, q = q, A = A, l = l, u = u)

results = OSQP.solve!(prob)
# Simulate in closed loop
nsim = 5000
for i = 1:nsim
    results = OSQP.solve!(prob)    # Check solver status

    # Apply first control input to the plant
    ctrl = results.x[(N + 1) * nx + 1:(N + 1) * nx + nu]
    global x0 = Ad * x0 + Bd * ctrl

    # Update initial state
    global l[1:nx] = -x0
    global u[1:nx] = -x0
    OSQP.update!(prob, l = l, u = u)
end
