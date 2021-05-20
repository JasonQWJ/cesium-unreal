// Copyright 2020-2021 CesiumGS, Inc. and Contributors

#pragma once

#include <glm/glm.hpp>

/**
 * @brief Vector math utility functions.
 *
 * The functions in this class mainly perform conversions between
 * the `glm` types and the Unreal Engine types, as well as basic,
 * frequently used mathematical operations on these types.
 *
 * As far as possible these functions will internally perform the
 * computations with `double` precision.
 *
 */
class VecMath {
public:
  /**
   * @brief Create a `glm` 4x4 matrix from the given `FMatrix`.
   *
   * @param m The `FMatrix`.
   * @return The `glm` matrix.
   */
  static glm::dmat4 createMatrix4D(const FMatrix& m) noexcept;

  /**
   * @brief Create a `glm` 4x4 matrix from the given `FMatrix`.
   *
   * This will use the elements of the given matrix, but replace
   * the translation column with the given translation vector.
   *
   * @param m The `FMatrix`.
   * @param translation The translation vector.
   * @return The `glm` matrix.
   */
  static glm::dmat4
  createMatrix4D(const FMatrix& m, const glm::dvec3& translation) noexcept;

  /**
   * @brief Create a `glm` 4x4 matrix from the given `FMatrix`.
   *
   * This will use the elements of the given matrix, but replace
   * the translation column with the given translation.
   *
   * @param m The `FMatrix`.
   * @param tx The translation in x-direction
   * @param ty The translation in y-direction
   * @param tz The translation in z-direction
   * @param tw The w-component of the translation, usually 1.0
   * @return The `glm` matrix.
   */
  static glm::dmat4 createMatrix4D(
      const FMatrix& m,
      double tx,
      double ty,
      double tz,
      double tw) noexcept;

  /**
   * @brief Create a `glm` 4x4 matrix from the given `FMatrix`.
   *
   * This will use the elements of the given matrix, but replace
   * the translation column with the given translation vector.
   *
   * @param m The `FMatrix`.
   * @param translation The translation vector.
   * @return The `glm` matrix.
   */
  static glm::dmat4
  createMatrix4D(const FMatrix& m, const glm::dvec4& translation) noexcept;

  /**
   * @brief Create a translation matrix from the given vector.
   *
   * This will return an identity matrix where the
   * the translation column is set to be the given
   * translation vector.
   *
   * @param translation The translation vector.
   * @return The `glm` matrix.
   */
  static glm::dmat4 createTranslationMatrix4D(
      double tx,
      double ty,
      double tz,
      double tw) noexcept;

  /**
   * @brief Create a `glm` vector from the given `FVector`.
   *
   * @param v The `FVector`
   * @return The `glm` vector
   */
  static glm::dvec3 createVector3D(const FVector& v) noexcept;

  /**
   * @brief Create a `glm` vector from the given `FIntVector`.
   *
   * @param v The `FIntVector`
   * @return The `glm` vector
   */
  static glm::dvec3 createVector3D(const FIntVector& v) noexcept;

  /**
   * @brief Create a `FMatrix` from the given `glm` matrix.
   *
   * The result will be an identity matrix, with the upper-left 3x3
   * matrix to be set to the given input.
   *
   * @param m The `glm`matrix.
   * @return The `FMatrix`.
   */
  static FMatrix createMatrix(const glm::dmat3& m) noexcept;

  /**
   * @brief Create a `FMatrix` from the given `glm` matrix.
   *
   * @param m The `glm`matrix.
   * @return The `FMatrix`.
   */
  static FMatrix createMatrix(const glm::dmat4& m) noexcept;

  /**
   * @brief Create a `FMatrix` from the given `glm` columns
   *
   * The result will be an identity matrix, with the upper-left 3x3
   * matrix to be set to the given columns
   *
   * @param column0 The first column
   * @param column1 The second column
   * @param column2 The third column
   * @return The `FMatrix`.
   */
  static FMatrix createMatrix(
      const glm::dvec3& column0,
      const glm::dvec3& column1,
      const glm::dvec3& column2) noexcept;

  /**
   * @brief Add the given `FVector` and `FIntVector`, to create a `glm` vector.
   *
   * This will internally perform the computation with `double` precision.
   *
   * @param f The `FVector`
   * @param i The `FIntVector`
   * @return The `glm` vector
   */
  static glm::dvec4 add4D(const FVector& f, const FIntVector& i) noexcept;

  /**
   * @brief Add the given `FIntVector` and `FVector`, to create a `glm` vector.
   *
   * This will internally perform the computation with `double` precision.
   *
   * @param i The `FIntVector`
   * @param f The `FVector`
   * @return The `glm` vector
   */
  static glm::dvec4 add4D(const FIntVector& i, const FVector& f) noexcept;

  /**
   * @brief Add the `glm` vector and `FIntVector`, to create a `glm` vector.
   *
   * @param d The `glm` vector
   * @param i The `FIntVector`
   * @return The `glm` vector
   */
  static glm::dvec4 add4D(const glm::vec4& d, const FIntVector& i) noexcept;

  /**
   * @brief Add the given `FVector` and `FIntVector`, to create a `glm` vector.
   *
   * This will internally perform the computation with `double` precision.
   *
   * @param f The `FVector`
   * @param i The `FIntVector`
   * @return The `glm` vector
   */
  static glm::dvec3 add3D(const FVector& f, const FIntVector& i) noexcept;

  /**
   * @brief Add the given `FIntVector` and `FVector`, to create a `glm` vector.
   *
   * This will internally perform the computation with `double` precision.
   *
   * @param i The `FIntVector`
   * @param f The `FVector`
   * @return The `glm` vector
   */
  static glm::dvec3 add3D(const FIntVector& i, const FVector& f) noexcept;

  /**
   * @brief Add the `glm` vector and `FIntVector`, to create a `glm` vector.
   *
   * @param d The `glm` vector
   * @param i The `FIntVector`
   * @return The `glm` vector
   */
  static glm::dvec3 add3D(const glm::vec3& d, const FIntVector& i) noexcept;

  /**
   * @brief Subtract the given `FIntVector` from the given `FVector`, to create
   * a `glm` vector.
   *
   * This will internally perform the computation with `double` precision.
   *
   * @param f The `FVector`
   * @param i The `FIntVector`
   * @return The `glm` vector
   */
  static glm::dvec4 subtract4D(const FVector& f, const FIntVector& i) noexcept;

  /**
   * @brief Subtract the given `FVector` from the given `FIntVector`, to create
   * a `glm` vector.
   *
   * This will internally perform the computation with `double` precision.
   *
   * @param i The `FIntVector`
   * @param f The `FVector`
   * @return The `glm` vector
   */
  static glm::dvec4 subtract4D(const FIntVector& i, const FVector& f) noexcept;

  /**
   * @brief Subtract the given `FIntVector` from the given `FVector`, to create
   * a `glm` vector.
   *
   * This will internally perform the computation with `double` precision.
   *
   * @param f The `FVector`
   * @param i The `FIntVector`
   * @return The `glm` vector
   */
  static glm::dvec3 subtract3D(const FVector& f, const FIntVector& i) noexcept;

  /**
   * @brief Subtract the given `FVector` from the given `FIntVector`, to create
   * a `glm` vector.
   *
   * This will internally perform the computation with `double` precision.
   *
   * @param i The `FIntVector`
   * @param f The `FVector`
   * @return The `glm` vector
   */
  static glm::dvec3 subtract3D(const FIntVector& i, const FVector& f) noexcept;
};
