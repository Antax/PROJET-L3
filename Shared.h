#ifndef SP_SHARED_H
#define SP_SHARED_H

#include <cstddef>
#include <utility>
#include <map>

namespace sp {

  /**
   * @brief Smart shared pointer class
   */
  template<typename T>
  class Shared {
  public:
    /**
     * @brief Constructor takes a dynamic pointer
     */
    Shared(T* ptr = nullptr) {
    }

    /**
     * @brief Destructor
     */
    ~Shared() {
    }

    /**
     * @brief Copy constructor
     */
    Shared(const Shared<T>& other) {
    }

    /**
     * @brief Move constructor
     */
    Shared(Shared&& other) {
    }

    /**
     * @brief Copy assignment
     */
    Shared& operator=(const Shared& other) {
      return *this;
    }

    /**
     * @brief Move assignment
     */
    Shared& operator=(Shared&& other) {
      return *this;
    }

    /**
     * @brief Get the raw pointer
     */
    T* get() {
      return nullptr;
    }

    /**
     * @brief Get a reference on pointed data
     */
    T& operator*() {
      return T();
    }

    /**
     * @brief Get the raw pointer
     */
    T* operator->() {
      return nullptr;
    }

    /**
     * @brief Get the reference number on raw data
     */
    std::size_t count() const {
      return 0;
    }

    /**
     * @brief Get the number of Shared pointed on the current pointer
     */
    bool exists() const {
      return false;
    }

    template<typename> friend class Weak;

  private:
    // implementation defined
  };
}

#endif // SP_SHARED_H