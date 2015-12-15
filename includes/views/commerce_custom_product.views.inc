<?php
/**
 * @file
 * Provide shopping cart related Views integration.
 */

/**
 * Implements hook_views_data().
 */
function commerce_custom_product_views_data() {
  $data = array();
  $data['commerce_line_item']['product_customization_description'] = array(
    'title' => t('Product Customization'),
    'help' => t('The product customization.'),
    'field' => array(
      'title' => t('Show the customizing description'),
      'help' => t('Display the product customizing description configured in the field'),
      'handler' => 'commerce_custom_product_handler_field_product_customization_description',
    ),
  );

  $data['commerce_line_item']['product_customization_order'] = array(
    'title' => t('Sort customization line items'),
    'help' => t('The order of customization line items.'),
    'sort' => array(
      'handler' => 'commerce_custom_product_handler_sort_product_customization',
    ),
  );

  return $data;
}
