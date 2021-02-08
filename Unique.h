#ifndef SP_UNIQUE_H
#define SP_UNIQUE_H

namespace sp {

  /**
   * @brief Smart unique pointer
   */
  template<typename T>
  class Unique {
  public:
    /**
     * @brief Constructor takes a dynamic pointer
     */
    Unique(T* ptr = nullptr) {
      pointerino=ptr;
    }

    /**
     * @brief Destructor
     */
    ~Unique() {
      delete pointerino;
    }

    /**
     * @brief Copy constructor - deleted
     */
    Unique(const Unique<T>& other) = delete;

    /**
     * @brief Move constructor
     */
    Unique(Unique&& other) {
      std::swap(pointerino,other.pointerino);
      return pointerino;
    }

    /**
     * @brief Copy assignment - deleted
     */
    Unique& operator=(const Unique& other) = delete;

    /**
     * @brief Move assignment
     */
    Unique& operator=(Unique&& other) {
      return *this;
    }

    /**
     * @brief Get the raw pointer
     */
    T* get() {
      return pointerino;
    }

    /**
     * @brief Get a reference on pointed data
     */
    T& operator*() {
      return *pointerino;
    }

    /**
     * @brief Get the raw pointer
     */
    T* operator->() {
      return pointerino;
    }

    /**
     * @brief Check if the raw pointer exists
     */
    bool exists() const {
      return false;
    }

  private:
    // implementation defined
    T* pointerino;
  };
}

#endif // SP_UNIQUE_H