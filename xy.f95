! fortran 95

program ga

implicit none

integer, parameter :: POPULATION_SIZE = 100
integer, parameter :: PATH_LENGTH = 100
integer, parameter :: NUM_GENERATIONS = 1000

integer :: i, j, k

type point
    integer :: x
    integer :: y
end type point

type(point), allocatable :: paths(:,:)
type(point), allocatable :: new_paths(:,:)

allocate(paths(POPULATION_SIZE, PATH_LENGTH))
allocate(new_paths(POPULATION_SIZE, PATH_LENGTH))

! Generate a random population of paths
do i = 1, POPULATION_SIZE
    do j = 1, PATH_LENGTH
        paths(i,j)%x = random_number() * 100
        paths(i,j)%y = random_number() * 100
    end do
end do

! Evaluate each path
do i = 1, POPULATION_SIZE
    paths(i,1)%x = 0
    paths(i,1)%y = 0
    paths(i,PATH_LENGTH)%x = 99
    paths(i,PATH_LENGTH)%y = 99
    write(*,*) evaluate(paths(i,:), PATH_LENGTH)
end do

! Perform genetic algorithm
do i = 1, NUM_GENERATIONS
    ! Selection
    do j = 1, POPULATION_SIZE
        integer :: r1, r2
        r1 = random_number() * POPULATION_SIZE
        r2 = random_number() * POPULATION_SIZE
        if (evaluate(paths(r1,:), PATH_LENGTH) < evaluate(paths(r2,:), PATH_LENGTH)) then
            do k = 1, PATH_LENGTH
                new_paths(j,k) = paths(r1,k)
            end do
        else
            do k = 1, PATH_LENGTH
                new_paths(j,k) = paths(r2,k)
            end do
        end if
    end do
    
    ! Crossover
    do j = 1, POPULATION_SIZE, 2
        crossover(new_paths(j,:), new_paths(j+1,:), paths(j,:), PATH_LENGTH)
        crossover(new_paths(j+1,:), new_paths(j,:), paths(j+1,:), PATH_LENGTH)
    end do
    
    ! Mutation
    do j = 1, POPULATION_SIZE
        mutate(paths(j,:), PATH_LENGTH)
    end do
    
    ! Evaluation
    do j = 1, POPULATION_SIZE
        paths(j,1)%x = 0
        paths(j,1)%y = 0
        paths(j,PATH_LENGTH)%x = 99
        paths(j,PATH_LENGTH)%y = 99
        write(*,*) evaluate(paths(j,:), PATH_LENGTH)
    end do
end do

! Find the shortest path
integer :: best_distance, best_path
best_distance = -1
best_path = -1
do i = 1, POPULATION_SIZE
    integer :: d
    d = evaluate(paths(i,:), PATH_LENGTH)
    if (best_distance == -1 .or. d < best_distance) then
        best_distance = d
        best_path = i
    end if
end do

! Print the shortest path
write(*,*) "Shortest path: "
do i = 1, PATH_LENGTH
    write(*,*) paths(best_path,i)%x, paths(best_path,i)%y
end do

contains

function distance(p1, p2) result(d)
    type(point), intent(in) :: p1, p2
    integer :: d
    d = abs(p1%x - p2%x) + abs(p1%y - p2%y)
end function distance

function evaluate(path, length) result(d)
    type(point), dimension(length), intent(in) :: path
    integer, intent(in) :: length
    integer :: d, i
    d = 0
    do i = 2, length
        d = d + distance(path(i-1), path(i))
    end do
    d = d
end function evaluate

subroutine mutate(path, length)
    type(point), dimension(length), intent(inout) :: path
    integer, intent(in) :: length
    integer :: i1, i2
    type(point) :: tmp
    i1 = random_number() * length
    i2 = random_number() * length
    tmp = path(i1)
    path(i1) = path(i2)
    path(i2) = tmp
end subroutine mutate

subroutine crossover(path1, path2, new_path, length)
    type(point), dimension(length), intent(in) :: path1, path2
    type(point), dimension(length), intent(out) :: new_path
    integer, intent(in) :: length
    integer :: i, r
    do i = 1, length
        r = random_number() * 2
        if (r == 0) then
            new_path(i) = path1(i)
        else
            new_path(i) = path2(i)
        end if
    end do
end subroutine crossover

end program ga

*/
