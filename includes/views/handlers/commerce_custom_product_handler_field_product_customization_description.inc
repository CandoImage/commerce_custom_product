<?php
/**
 * @file
 * Views integration for commerce_custom_product.
 */

/**
 * Field handler to show the customizing description of an extra line item.
 *
 * Use this as group to work with use_multi option.
 */
class commerce_custom_product_handler_field_product_customization_description extends views_handler_field {

  public function construct() {
    parent::construct();
    $this->additional_fields['line_item_id'] = 'line_item_id';
  }

  /**
   * Don't add a field to the query.
   */
  public function query() {
    $this->ensure_my_table();
    $this->add_additional_fields();
  }

  public function render($values) {
    $customizing_settings = $this->get_value($values);
    $description = array(
      '#type' => 'item',
    );
    if (!empty($customizing_settings)) {
      $description['#name'] = 'customizing-delta-' . $customizing_settings['delta'];
      if (!empty($customizing_settings['label'])) {
        $description['#title'] = '<p class="customizing-label">' . check_plain($customizing_settings['label']) . '</p>';
      }
      if (!empty($customizing_settings['description'])) {
        $description['#description'] = filter_xss($customizing_settings['description']);
      }
    }
    return drupal_render($description);
  }

  /**
   * Returns the customizing settings with the description and label in it.
   */
  public function get_value($values, $field = NULL) {
    if (isset($field)) {
      return parent::get_value($values, $field);
    }
    $line_item_id = parent::get_value($values, 'line_item_id');
    if ($extra_line_item = commerce_line_item_load($line_item_id)) {
      if ($customizing_settings = commerce_custom_product_get_customizing_settings_from_line_item($extra_line_item)) {
        return $customizing_settings;
      }
    }
    return array();
  }

}
