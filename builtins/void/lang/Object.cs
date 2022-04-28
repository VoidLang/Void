package void.lang;

/**
 * Represents the rood class of the class hierarchy.
 * Each objects' superclass extends Object.
 *
 * @author Abel
 * @since 1.0 
 */
public class Object {
    /**
     * Returns the runtime class of this Object.
     */
    public final native Class<?> getClass();

    /**
     * Returns a hash code value for the object.
     */
    public native int hashCode();

    /**
     * Indicates whether the given object is equal to this one.
     */
    public boolean equals(Object other) {
        return other == this;
    }

     /**
     * Makes a string representation of the object.
     */
    public native String toString();

    /**
     * Determines whether the address of the given object and 
     * the address of the current instance (this) equals.
     */
    public native boolean operator==(Object other);
}