

template <size_t M>
std::ostream& operator<< (std::ostream& os, const PolyVector<M>& poly_vector) {
    size_t ctr = 0;
    for(const auto& poly : poly_vector._poly_vector) {
        os << "Vector[" << (int)ctr << "]:" << std::endl; 
        os << poly;
        ctr++;
    }
    return os;
}